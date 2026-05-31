make || exit

philo="philo"
output_file="output.logs"
error_file="error.logs"

red="\001\033[31m\002"
blue="\001\033[34m\002"
green="\001\033[32m\002"
yellow="\001\033[93m\002"
default="\001\033[0m\002"

wrong_test=0

check_death()
{
	if tail -n1 $output_file | grep -q "died"
	then
		tail -n1 $output_file | awk '{print $2}'
	else
		echo 0
	fi
}

test()
{
	echo -e "./$philo $1 $2 $3 $4 $5"
	unbuffer ./$philo $1 $2 $3 $4 $5 >$output_file 2>$error_file
	errors=$(wc -l $error_file | awk '{print $1;}')
	if [ $errors -ne 0 ]; then
		echo -e "\001\033[31m\002"
		cat error.log
		echo -e "\001\033[0m\002"
	fi
	who_died=$(check_death)
	return $who_died
}

test_format()
{
	errors=$(grep -vE "eating|sleeping|thinking|died|fork" $output_file)
	if [ "$errors" != "" ]
	then
		echo -e "${red}This shouldn't happen:\n$errors${default}"
	fi
}

save_logs()
{
	wrong_test=$((wrong_test + 1))
	file_name=$(echo "output$wrong_test.logs")
	cp $output_file $file_name
	echo -e "\n./$philo $1" >> $file_name
	echo "output saved in $file_name"
}

should_not_die()
{
	test $1 $2 $3 $4 $5
	who_died=$?
	test_format
	if [ $who_died -eq 0 ]; then
		echo -e "${green}"No one died"${default}"
	else
		echo -e "${red}"Someone died"${default}"
		save_logs "$1 $2 $3 $4 $5"
	fi
	echo
}

should_die()
{
	test $1 $2 $3 $4 $5
	who_died=$?
	test_format
	if [ $who_died -ne 0 ]; then
		echo -e "${green}"Someone died"${default}"
	else
		echo -e "${red}"No one died"${default}"
		save_logs "$1 $2 $3 $4 $5"
	fi
	echo
}

valgrind_logs=0
test_leaks()
{
	echo -e "valgrind ./$philo $1 $2 $3 $4 $5"
	valgrind_file="valgrind.logs"
	supervalgrind="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --trace-children=no --child-silent-after-fork=no --log-file=$valgrind_file"
	$supervalgrind ./$philo $1 $2 $3 $4 $5 >$output_file 2>$error_file
	valgrind_errors=$(cat $valgrind_file | grep "in use at exit" | grep -v " 0 bytes")
	if [ "$valgrind_errors" != "" ]
	then
		valgrind_logs=$(($valgrind_logs + 1))
		echo -e "${red}Valgrind detected errors, saving them in 'valgrind_$valgrind_logs.logs'"
		cp $valgrind_file valgrind_$valgrind_logs.logs
	else
		echo -e "${green}OK${default}"
	fi
	echo
}

count_meals()
{
	test $1 $2 $3 $4 $5
	for i in $(seq 1 $1)
	do
		meals=$(grep " $i " $output_file | grep eating | wc -l)
		if [ $meals -lt $5 ]
		then
			echo -e "\n${red}Not enought philos has eaten ($i,$meals)"
			save_logs
		else
			echo -ne "${green}OK${default} "
		fi
	done
	echo -e "\n"
}

for binary in "philo" "philo_bonus"
do
	philo=$binary
	echo -e "${yellow}\n$philo${default}\n"

	echo -e "${blue}Counting meals${default}"

	count_meals 4 400 100 100 10
	count_meals 5 400 100 100 10
	count_meals 20 400 100 100 29
	count_meals 27 400 100 100 29
	count_meals 5 400 100 100 200
	count_meals 4 400 100 100 200
	count_meals 200 400 100 100 30
	count_meals 201 400 100 100 30

	echo -e "${blue}Should die${default}"

	should_die 4 99 200 200 2
	should_die 4 399 200 200 2
	should_die 4 399 200 200 2
	should_die 4 399 200 200 2
	should_die 4 399 200 200 2
	should_die 4 399 200 200 2
	should_die 4 399 200 200 2
	should_die 4 399 200 200 2
	should_die 5 599 200 200 2
	should_die 1 500 200 200 2

	echo -e "${blue}Should not die${default}"

	should_not_die 4 410 200 200 50
	should_not_die 5 610 200 200 50
	should_not_die 200 450 200 200 30
	should_not_die 201 650 200 200 30

	echo -e "${blue}Hope not die${default}"

	should_not_die 200 410 200 200 30
	should_not_die 201 610 200 200 30

	should_not_die 200 420 200 200 30
	should_not_die 201 620 200 200 30

	should_not_die 200 430 200 200 30
	should_not_die 201 630 200 200 30

	echo -e "${blue}Leaks${default}"

	test_leaks 4 10 100 100
	test_leaks 5 10 100 100
	test_leaks 1 2000 100 100
	test_leaks 0 2000 100 100
	test_leaks 4 4000 100 100 10
	test_leaks 5 4000 100 100 10
	test_leaks 50 4000 100 100 10
	test_leaks 51 4000 100 100 10
	test_leaks 100 4000 100 100 5
	test_leaks 101 4000 100 100 5

done

rm -f output.logs error.logs