#!/bin/bash

# Jamie Lewis
# CST-221
# 1/16/2021
# Instructor John Zupan

# This script manages users by reading a username and encrypted password
# from a file. The users in the file can be added or removed, as long as
# they do not have any open processes. The bash command requires sudo and
# three arguments incuding a file name, group name to add the users to, and
# a flag to indicate adding or removing the users. Messages are output to
# the terminal to indicate errors or what the script is doing. Home directories
# are created and deleted with users. Blank lines in the file are skipped.
# One thing this script does not do is delete a group if it is left empty
# after users are removed, but that was not part of the assignment
# instructions anyway.

# Arguments: File, Group, Operation Flag
filename=$1
grp=$2
opflag=$3

# Check for correct number of args
if [ $# != 3 ]; then
	# Incorrect commands error output
	echo "Incorrect commands. Use [file] [group] [-a|-r]";
	exit 1
fi

# Read from file
while read -r user pw;
	do
	user=$user
	pw=$pw
	# Is user ROOT?
	if [ $(id -u) -eq 0 ];
		then

		# Add user if 3rd arg is -a
		if [[ $opflag == "-a" ]]; then
				# Check if group exists
				if egrep -i $grp /etc/group >/dev/null; then
					# If group does exist
					echo "Group already exists."
				else
					# Add group if it does not exist
					groupadd $grp
					echo "Group created."
				fi
				# Add users to group, checking for blank lines in file
				if [[ $user != '' && $pw != '' ]]; then
					useradd -m --groups $grp -p $pw $user
					echo "$user created with a home directory."
				fi

		#Delete users if 3rd arg is -r
		elif [[ $opflag == "-r" ]]; then
			# Remove users from group, checking for blank lines in file
			if [[ $user != '' && $pw != '' ]]; then
				# Check if user exists
				if id -u "$user" >/dev/null; then
					# If user exists, -r deletes their home directory
					userdel -r $user 2>/dev/null
					echo "$user deleted along with their home directory."
				else
					# If user did not exist
			    	echo "$user does not exist."
				fi
			fi

		# If 3rd command was not -a or -r
		else
		echo "Incorrect Operation Flag. Use [-a|-r] for 3rd and final command."
		exit 1
	fi
	else
		#Not ROOT user error
		echo "Must be root to add or remove users and directories!"
		exit 1
	fi

# Close File
done < "$filename"
