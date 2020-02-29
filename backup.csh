#!/usr/bin/csh -x

set BACKUP_DIR = /usr/src/backup.d/qtnqc.d

/usr/bin/cp -p *.cc $BACKUP_DIR/.
/usr/bin/cp -p *.h $BACKUP_DIR/.
/usr/bin/cp -p TO-DO $BACKUP_DIR/.
/usr/bin/cp -p README $BACKUP_DIR/.
/usr/bin/cp -p qtnqc.pro $BACKUP_DIR/.
/usr/bin/cp -p backup.csh $BACKUP_DIR/.
/usr/bin/cp -pr icons $BACKUP_DIR/.
/usr/bin/cp -p KNOWN-ISSUES $BACKUP_DIR/.
