web_path = /home/groups/l/li/libsigc/htdocs/

post-html:
	rsync -avz --rsh ssh --cvs-exclude .htaccess $$USER,libsigc@web.sourceforge.net:$(web_path)

#post-html:
#	scp $$SSH_OPT -r *.shtml *.css fragments images $$USER@shell.sourceforge.net:$(web_path)
