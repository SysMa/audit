/* Modified for use in Yafc by Martin Hedenfalk <mhe@stacken.kth.se>
 *
 * Original copyright:
 *
 * Copyright (c) 2001 Damien Miller.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "syshdr.h"
#include "ftp.h"
#include "strq.h"
#include "gvars.h"
#include "buffer.h"
#include "bufaux.h"
/*#include "pathnames.h"*/
#include "ssh_ftp.h"
#include "rfile.h"

int ssh_open_url(url_t *urlp)
{
	char *sftp = gvSFTPServerProgram;
	char *s;
	int r;

	args_clear(ftp->ssh_args);

	if(urlp->sftp_server)
		sftp = urlp->sftp_server;

	args_push_back(ftp->ssh_args, gvSSHProgram);
	if(gvSSHOptions && strlen(gvSSHOptions))
		args_push_back(ftp->ssh_args, gvSSHOptions);
	args_push_back(ftp->ssh_args, urlp->hostname);

	r = get_username(urlp, gvUsername, false);
	if(r != 0)
		return r;

	if(urlp->username) {
		args_push_back(ftp->ssh_args, "-l");
		args_push_back(ftp->ssh_args, urlp->username);
	} else {
		/* we need a username, otherwise other functions will fail
		 */
		urlp->username = xstrdup(gvUsername);
	}

	if(urlp->port) {
		char *p;
		asprintf(&p, "%d", urlp->port);
		args_push_back(ftp->ssh_args, "-p");
		args_push_back(ftp->ssh_args, p);
		free(p);
	}

	if(ftp_get_verbosity() == vbDebug)
		args_push_back(ftp->ssh_args, "-v");

	/* no subsystem if the server-spec contains a '/' */
	if(sftp == 0 || strchr(sftp, '/') == 0)
		args_push_back(ftp->ssh_args, "-s");

	/* Otherwise finish up and return the arg array */
	if(sftp != 0)
		args_push_back(ftp->ssh_args, sftp);
	else
		args_push_back(ftp->ssh_args, "sftp");

	s = args_cat2(ftp->ssh_args, 0);
	ftp_trace("SSH args: %s\n", s);
	if(ftp_get_verbosity() == vbDebug)
		fprintf(stderr, "SSH args: %s\n", s);
	free(s);

	args_add_null(ftp->ssh_args);

	if(ssh_connect(ftp->ssh_args->argv, &ftp->ssh_in, &ftp->ssh_out,
				   &ftp->ssh_pid) == -1)
		{
			return -1;
		}

	ftp->ssh_version = ssh_init();
	if(ftp->ssh_version == -1) {
		ftp_err("Couldn't initialise connection to server\n");
		return -1;
	}

	ftp->connected = true;
	ftp->loggedin = true;

	ftp->homedir = ftp_getcurdir();

	url_destroy(ftp->url);
	ftp->url = url_clone(urlp);

	ftp->curdir = xstrdup(ftp->homedir);
	ftp->prevdir = xstrdup(ftp->homedir);
	if(ftp->url->directory)
		ftp_chdir(ftp->url->directory);

	ftp_get_feat();

	return 0;
}

char *ssh_getcurdir(void)
{
	char *ret = ssh_realpath(".");
	if(!ret)
		return xstrdup("CWD?");
	stripslash(ret);
	return ret;
}

int ssh_chdir(const char *path)
{
	Attrib *aa;
	char *p = ftp_path_absolute(path);
	bool isdir = false;

	/* First check if this file is cached and is a directory, else we
	 * need to stat the file to see if it really is a directory
	 */

	stripslash(p);
	isdir = (ftp_cache_get_directory(p) != 0);
	if(!isdir) {
		rfile *rf = ftp_cache_get_file(p);
		isdir = (rf && risdir(rf));
	}
	if(!isdir) {
		if ((aa = ssh_stat(p)) == 0) {
			free(p);
			return -1;
		}
		if (!(aa->flags & SSH2_FILEXFER_ATTR_PERMISSIONS)) {
			ftp_err("Can't change directory: Can't check target");
			free(p);
			return -1;
		}
		if (!S_ISDIR(aa->perm)) {
			ftp_err("%s: not a directory\n", p);
			free(p);
			return -1;
		}
	}
	ftp_update_curdir_x(p);

	return 0;
}

int ssh_cdup(void)
{
	return ssh_chdir("..");
}

int ssh_mkdir_verb(const char *path, verbose_t verb)
{
	Attrib a;
	u_int status, id;
	char *abspath;

	attrib_clear(&a);
	a.flags |= SSH2_FILEXFER_ATTR_PERMISSIONS;
	a.perm = 0777;

	id = ftp->ssh_id++;
	abspath = ftp_path_absolute(path);
	stripslash(abspath);
	ssh_send_string_attrs_request(id, SSH2_FXP_MKDIR, abspath,
								  strlen(abspath), &a);

	status = ssh_get_status(id);
	if(status != SSH2_FX_OK) {
		ftp_err("Couldn't create directory: %s\n", fx2txt(status));
		free(abspath);
		return -1;
	}
	ftp_cache_flush_mark_for(abspath);
	free(abspath);

	return 0;
}

int ssh_rmdir(const char *path)
{
	char *p;
	u_int status, id;

	p = ftp_path_absolute(path);
	stripslash(p);

	id = ftp->ssh_id++;
	ssh_send_string_request(id, SSH2_FXP_RMDIR, p, strlen(p));

	status = ssh_get_status(id);
	if(status != SSH2_FX_OK) {
		ftp_err("Couldn't remove directory: %s\n", fx2txt(status));
		free(p);
		return -1;
	}
	ftp_cache_flush_mark(p);
	ftp_cache_flush_mark_for(p);
	free(p);

	return 0;
}

int ssh_unlink(const char *path)
{
	u_int status, id;

	id = ftp->ssh_id++;
	ssh_send_string_request(id, SSH2_FXP_REMOVE, path, strlen(path));
	status = ssh_get_status(id);
	if(status != SSH2_FX_OK) {
/*		ftp_err("Couldn't delete file: %s\n", fx2txt(status));*/
		ftp->code = ctError;
		ftp->fullcode = 500;
		return -1;
	} else
		ftp_cache_flush_mark_for(path);

	return 0;
}

/* FIXME: path must be absolute (check)
 */
int ssh_chmod(const char *path, const char *mode)
{
	Attrib a;

	attrib_clear(&a);
	a.flags |= SSH2_FILEXFER_ATTR_PERMISSIONS;
	a.perm = strtoul(mode, 0, 8);
	if(ssh_setstat(path, &a) == -1)
		return -1;
	ftp_cache_flush_mark_for(path);
	return 0;
}

int ssh_idle(const char *idletime)
{
	return 0;
}

int ssh_noop(void)
{
	return 0;
}

int ssh_help(const char *arg)
{
	return 0;
}

unsigned long ssh_filesize(const char *path)
{
	Attrib *a = ssh_stat(path);
	return a ? a->size : 0;
}

rdirectory *ssh_read_directory(const char *path)
{
	rdirectory *rdir;
	int i;
	SFTP_DIRENT **dir;
	char *p = ftp_path_absolute(path);
	stripslash(p);

	if(ssh_readdir(p, &dir) != 0) {
		free(p);
		return 0;
	}

	rdir = rdir_create();

	ftp_trace("*** start parsing directory listing ***\n");

	for(i = 0; dir[i]; i++) {
		rfile *rf;
		char *e, *cf = dir[i]->longname;

		ftp_trace("%s\n", dir[i]->longname);

		rf = rfile_create();

		rf->perm = perm2string(dir[i]->a.perm);

		e = strqsep(&cf, ' ');  /* skip permissions */
		e = strqsep(&cf, ' ');  /* nhl? */
/*		if(ftp->ssh_version > 2) {*/
			rf->nhl = atoi(e);
/*		} else*/
/*			rf->nhl = 0;*/
#if 1
		e = strqsep(&cf, ' ');
		rf->owner = xstrdup(e);
		e = strqsep(&cf, ' ');
		rf->group = xstrdup(e);
#else
		asprintf(&rf->owner, "%d", dir[i]->a.uid);
		asprintf(&rf->group, "%d", dir[i]->a.gid);
#endif

		asprintf(&rf->path, "%s/%s", p, dir[i]->filename);
		rf->mtime = dir[i]->a.mtime;
		if(rf->mtime == 0) {
			char *m, *d, *y;
			while(e && month_number(e) == -1)
				e = strqsep(&cf, ' ');
			if(e) {
				m = e;
				d = strqsep(&cf, ' ');
				y = strqsep(&cf, ' ');
				ftp_trace("parsing time: m:%s d:%s y:%s\n", m, d, y);
				rfile_parse_time(rf, m, d, y);
			}
		}
		rf->date = time_to_string(rf->mtime);
		rf->size = dir[i]->a.size;
		rfile_parse_colors(rf);

		rf->link = 0;
		if(rislink(rf) && ftp->ssh_version > 2)
			rf->link = ssh_readlink(rf->path);

		list_additem(rdir->files, (void *)rf);
	}
	ftp_trace("*** end parsing directory listing ***\n");

	ssh_free_dirents(dir);

	rdir->path = p;

	ftp_trace("added directory '%s' to cache\n", p);
	list_additem(ftp->cache, rdir);

	return rdir;
}

int ssh_rename(const char *oldname, const char *newname)
{
	Buffer msg;
	u_int status, id;
	char *on, *nn;

	buffer_init(&msg);

	on = ftp_path_absolute(oldname);
	nn = ftp_path_absolute(newname);
	stripslash(on);
	stripslash(nn);

	/* Send rename request */
	id = ftp->ssh_id++;
	buffer_put_char(&msg, SSH2_FXP_RENAME);
	buffer_put_int(&msg, id);
	buffer_put_cstring(&msg, on);
	buffer_put_cstring(&msg, nn);
	ssh_cmd( &msg);
	buffer_free(&msg);

	status = ssh_get_status(id);
	if(status != SSH2_FX_OK) {
		ftp_err("Couldn't rename file \"%s\" to \"%s\": %s\n",
				on, nn, fx2txt(status));
		free(on);
		free(nn);
		return -1;
	}

	ftp_cache_flush_mark_for(on);
	ftp_cache_flush_mark_for(nn);
	free(on);
	free(nn);
	return 0;
}

time_t ssh_filetime(const char *filename)
{
	Attrib *a = ssh_stat(filename);
	return a ? a->mtime : -1;
}

int ssh_list(const char *cmd, const char *param, FILE *fp)
{
	ftp_err("ssh_list() not implemented yet\n");

	return -1;
}

void ssh_pwd(void)
{
	printf("%s\n", ftp->curdir);
}

int ssh_do_receive(const char *infile, FILE *fp, getmode_t mode,
				   ftp_transfer_func hookf)
{
	int r;
	rfile *f;
	u_int64_t offset = ftp->restart_offset;

	ftp->ti.size = ftp->ti.restart_size = offset;
	ftp->restart_offset = 0L;

	f = ftp_cache_get_file(infile);
	if(f)
		ftp->ti.total_size = f->size;
	else
		ftp->ti.total_size = ssh_filesize(infile);

	r = ssh_recv_binary(infile, fp, hookf, offset);

	transfer_finished();

	return (r == 0 && !ftp->ti.ioerror && !ftp->ti.interrupted) ? 0 : -1;
}

int ssh_send(const char *path, FILE *fp, putmode_t how,
			 transfer_mode_t mode, ftp_transfer_func hookf)
{
	int r;
	long offset = ftp->restart_offset;
	char *p;

	reset_transfer_info();
	ftp->ti.size = ftp->ti.restart_size = offset;
	ftp->restart_offset = 0L;

	ftp->ti.transfer_is_put = true;

	if(how == putUnique) {
		ftp_err("Unique put with SSH not implemented yet\n");
		return -1;
#if 0
		/* try to figure out remote filename */
		char *e = strstr(ftp->reply, " for ");
		if(e) {
			int l;
			e += 5;
			l = strlen(e);
			if(l) {
				free(ftp->ti.local_name);
				if(*e == '\'')
					ftp->ti.local_name = xstrndup(e+1, l-3);
				else
					ftp->ti.local_name = xstrndup(e, l-1);
				ftp_trace("parsed unique filename as '%s'\n",
						  ftp->ti.local_name);
			}
		}
#endif
	}

	p = ftp_path_absolute(path);
	stripslash(p);

	ftp_cache_flush_mark_for(p);

	if(how == putAppend) {
		ftp_set_tmp_verbosity(vbNone);
		offset = ftp_filesize(p);
	}

	r = ssh_send_binary(p, fp, hookf, offset);
	free(p);

	transfer_finished();

	return 0;
}
