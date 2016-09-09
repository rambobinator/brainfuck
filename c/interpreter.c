#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

enum bool {FALSE, TRUE};

#define USAGE "Usage: ./a.out code\n"
#define IS_LOOP_START(c) ((c == '[') ? TRUE : FALSE)
#define IS_LOOP_END(c) ((c == ']') ? TRUE : FALSE)
#define MEM_SIZE 666
#define UCHAR unsigned char

typedef struct	s_entry
{
	char		*code;
	int			code_len;
	int			*pos;
	int			step;
}				t_entry;

static void	init_bracket_tab(t_entry *entry, int l_s, int l_e){
	int		i = -1;
	int		j = -1;

	if (l_s != l_e)
		return ;
	entry->step = l_s;
	if ((entry->pos = (int *)malloc((sizeof(int) * (l_s * 2)))) == NULL)
		return ;
	while (entry->code[++i])
		if IS_LOOP_START(entry->code[i])
			entry->pos[++j] = i;
		else if IS_LOOP_END(entry->code[i])
			entry->pos[j + l_s] = i;
}

static void	init_entry(t_entry *entry){
	int		loop_start_len = 0;
	int		loop_stop_len = 0;

	while (entry->code[++entry->code_len])
		if IS_LOOP_START(entry->code[entry->code_len])
			loop_start_len++;
		else if IS_LOOP_END(entry->code[entry->code_len])
			loop_stop_len++;
	init_bracket_tab(entry, loop_start_len, loop_stop_len);
}

static UCHAR	*init_memory(){
	UCHAR		*mem;

	if ((mem = (UCHAR *)malloc(sizeof(UCHAR) * MEM_SIZE)) == NULL)
		return NULL;
	bzero(mem, MEM_SIZE);
	return (mem);
}

static int		get_matching_bracket(t_entry *e, int i){
	return ((i >= e->step) ?  e->pos[i - e->step]: e->pos[i + e->step]);
}

static void		run(char *code){
	t_entry		e;
	UCHAR		*mem;
	int			i = -1;

	bzero(&e, sizeof(e));
	e.code = code;
	init_entry(&e);
	mem = init_memory();
	while (++i < e.code_len){
		if (code[i] == '>')
			mem++;
		else if (code[i] == '<')
			mem--;
		else if (code[i] == '+')
			(*mem)++;
		else if (code[i] == '-')
			(*mem)--;
		else if (code[i] == '.')
			write(1, &mem, 1);
		else if (code[i] == ',')
			;/* GETLINE */
		else if ((code[i] == '[') && !(*mem))
			i = get_matching_bracket(&e, i);
		else if (code[i] == ']')
			i = get_matching_bracket(&e, i);
	}
}

int 			main(int ac, char **av){
	if (ac == 2)
		run(av[1]);
	else
		write(1, USAGE, 21);
	return (0);
}
