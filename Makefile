.PHONY: all clean fclean re bonus execute_bonus

CFLAGS = -Wall -Wextra -Werror
DFLAGS = -g3 -fsanitize=address

NAME = philo
DIR = src
DIR_OBJ = obj
SRC = *.c

NAME_BONUS = philo_bonus
DIR_BONUS = src_bonus
DIR_OBJ_BONUS = obj_bonus
SRC_NAMES =  input main manage_philos monitor routine utils
SRC_BONUS = $(SRC_NAMES:%=$(DIR_BONUS)/ft_philo_%_bonus.c)
OBJ_BONUS = $(SRC_NAMES:%=$(DIR_OBJ_BONUS)/ft_philo_%_bonus.o)


all : $(NAME)

$(DIR_OBJ)/%.o: $(DIR)/%.c | $(DIR_OBJ)
	cc $(CFLAGS) $^ -c -o $@

$(NAME): $(SRC)
	

bonus: $(NAME_BONUS)

$(DIR_OBJ_BONUS):
	mkdir $(DIR_OBJ_BONUS)

$(DIR_OBJ_BONUS)/%.o: $(DIR_BONUS)/%.c | $(DIR_OBJ_BONUS)
	cc $(CFLAGS) $^ -c -o $@

$(NAME_BONUS): $(OBJ_BONUS)
	cc $(CFLAGS) $^ -o $(NAME_BONUS)


NPHILOS ?= 5
TIME_TO_DIE ?= 602
TIME_TO_EAT ?= 200
TIME_TO_SLEEP ?= 100
NUMBER_ITERATIONS ?= 20

execute_bonus: $(NAME_BONUS)
	./$(NAME_BONUS) $(NPHILOS) $(TIME_TO_DIE) $(TIME_TO_EAT) $(TIME_TO_SLEEP) $(NUMBER_ITERATIONS)

clean:
	rm -f /dev/shm/sem.*
	rm -rf $(DIR_OBJ)
	rm -rf $(DIR_OBJ_BONUS)
# 	@ ipcrm --posix-semaphore /<name>

fclean: clean
	rm -f $(NAME) $(NAME_BONUS)

re: fclean all