#!/bin/bash

# Jamie Lewis
# CST-221
# 1/16/2021
# Instructor John Zupan

# This simple bash script checks the strength of a password by verifying
# that it is at least 8 characters long and contains at least one number
# and one special character. Specific messages are output to the terminal
# for errors or success.

# First arg variable
pwd="$1"

# Check that length is at least 8 characters
if [ ${#pwd} -lt 8 ];

	# If not long enough
	then echo "Password length must be at least 8 characters."

	# Must include at least one numeric value
	else if [[ "$pwd" =~ [0-9] ]] ;

		# Must include at least one special character
		then if [[ "$pwd" =~ ['@','#','$','%','^','&','+','-','='] ]];

			# If everything passes
			then echo "SUCCESS!"

		# If there is no special character
		else echo "The password must contain one special character: @#$%^&+-="
		fi

	# If there is no numeric value
	else echo "The password must contain at least one numeric character."
	fi
fi
