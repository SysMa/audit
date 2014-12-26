rm /var/log/audit/* -f
/etc/init.d/auditd restart
auditctl -a exit,always -S 1 -S 3 -S 18 -S 20 -S 296 -S 2  -S 41 -S 43 -S 44 -S 45 -S 46 -S 47 -S 56 -S 57 -S 58 -S 59 -S 2 -S 293 -S 22 -S 288 -S 0 -S 17 -S 19 -S 295 -S 82 -S 264 -S 87 -S 86 -F success=1 -F uid!=0
auditctl -a exit,always -S 1 -S 3 -S 18 -S 20 -S 296 -S 2  -S 41 -S 43 -S 44 -S 45 -S 46 -S 47 -S 56 -S 57 -S 58 -S 59 -S 2 -S 293 -S 22 -S 288 -S 0 -S 17 -S 19 -S 295 -S 82 -S 264 -S 87 -S 86 -F success=1 -F auid!=0
