.PHONY: all clean fclean re bonus execute_bonus

CFLAGS = -Wall -Wextra -Werror -g3
DFLAGS = -g3 -fsanitize=address

NAME = philo
DIR = src
DIR_OBJ = obj
SRC_NAMES = input main manage_philos routine utils setters getters init monitor
SRC = $(SRC_NAMES:%=$(DIR)/ft_philo_%.c)
OBJ = $(SRC_NAMES:%=$(DIR_OBJ)/ft_philo_%.o)

NAME_BONUS = philo_bonus
DIR_BONUS = src_bonus
DIR_OBJ_BONUS = obj_bonus
SRC_NAMES_BONUS =  input main manage_philos monitor routine utils setters getters
SRC_BONUS = $(SRC_NAMES_BONUS:%=$(DIR_BONUS)/ft_philo_%_bonus.c)
OBJ_BONUS = $(SRC_NAMES_BONUS:%=$(DIR_OBJ_BONUS)/ft_philo_%_bonus.o)


all : $(NAME) $(NAME_BONUS)

$(DIR_OBJ):
	mkdir $(DIR_OBJ)

$(DIR_OBJ)/%.o: $(DIR)/%.c | $(DIR_OBJ)
	cc $(CFLAGS) $^ -c -o $@

$(NAME): $(OBJ)
	cc $(CFLAGS) $^ -o $@
	

bonus: $(NAME_BONUS)

$(DIR_OBJ_BONUS):
	mkdir $(DIR_OBJ_BONUS)

$(DIR_OBJ_BONUS)/%.o: $(DIR_BONUS)/%.c | $(DIR_OBJ_BONUS)
	cc $(CFLAGS) $^ -c -o $@

$(NAME_BONUS): $(OBJ_BONUS)
	cc $(CFLAGS) $^ -o $@


NPHILOS ?= 5
TIME_TO_DIE ?= 602
TIME_TO_EAT ?= 200
TIME_TO_SLEEP ?= 100
NUMBER_ITERATIONS ?= 20

execute_bonus: $(NAME_BONUS)
	./$(NAME_BONUS) $(NPHILOS) $(TIME_TO_DIE) $(TIME_TO_EAT) $(TIME_TO_SLEEP) $(NUMBER_ITERATIONS)

debug:
	cc $(CFLAGS) $(DFLAGS) src/*.c -o philo

clean:
	rm -rf $(DIR_OBJ)
	rm -rf $(DIR_OBJ_BONUS)

fclean: clean
	rm -f $(NAME) $(NAME_BONUS)

re: fclean all