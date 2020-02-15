#Problema e aporximativ corecta,verificare pentru eroare/corect a trebui facut la pasul impar
function paritate()
{
	val=`expr $1 % 2 `
	if test $val == 0
	then
		return 0
	else
		return 1
	fi
}
function ultimulCuvant(){
	j=""
	for i in $1
	do
		j=$i
	done
	echo "$j" | cut -f 1 -d '.'
}
function traversareDirector(){
	for i in "$1"/*.log
	do
		test -h "$i"
		link=$?
		if test -f "$i"
		then
			if test $link == 1
			then
				nr=0
				ok2=1
				line=""
				while read linie
				do
					paritate $nr
					ok1=$?
					if test $ok1 == 1
					then
						text=`echo "$linie" | grep -E "^[A-Z][a-zA-Z ]*\.$" `
						if test $ok2 == 0 || test -z "$text"
						then
							nextLinie=`expr $nr + 1 `
							lastWord=`ultimulCuvant "$linie"`
							echo "Eroare la liniile $nr-$nextLinie $lastWord " >> "$2"
							ok2=1
						else
							echo "$linie" >> "$2"
							echo "$line" >> "$2"
							var=`echo "$linie"|tr '[:lower:]' '[:upper:]'`
							echo "$var" >> "$2"
 
						fi
					else
						line="$linie"
						impar=`echo "$linie" |  grep -E "^([0-9]|1[0-2]):([0-9]|[1-2][0-9]|3[0-1]):([1-9]|1[0-2]):200([0-9]|10)$" `
						if test -z impar
						then
							ok2=0
						fi


					fi
					nr=`expr $nr + 1 `

				done < "$i"
			fi
		fi
		
	done
	for i in "$1"/*
	do
		if test -d "$i"
		then
			traversareDirector "$i"
		fi
	done
	
}

traversareDirector "$1" "$2"