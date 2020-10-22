/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psabreto <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/14 23:13:59 by psabreto          #+#    #+#             */
/*   Updated: 2020/10/21 23:10:16 by psabreto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

typedef struct s_data
{
	int count;
	int num;
	char *str;
	unsigned int x_int;
	int is_width;//ширина
	int count_width;//сколько ширина
	int is_precison;//есть точность
	int count_precison; //сколько точность
	int is_procent;//есть процент или нет
	va_list args;
} t_data;

static void ft_putchar(char c)
{
	write(1, &c, 1);
}

static void ft_putstr(char *str)
{
	int i;

	i = 0;
	while(str[i] != '\0')
	{
		ft_putchar(str[i]);
		i++;
	}
}

int is_num(char s)
{
	if (s >= '0' && s <= '9')
		return (1);
	return (0);
}

static void ft_putchar_num(int num)
{
	if (num == -2147483648)
	{
		ft_putstr("-2147483648");
	}
	if(num < 0)
	{
		ft_putchar_num('-');
		num *= -1;
	}
	if (num > 9)
		ft_putchar_num(num / 10);
	ft_putchar(num % 10 + '0');
}

static int ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

static int count_num(int num)
{
	int i;

	i = (num < 0) ? 1 : 0;
	while (num != 0)
	{
		num /= 10;
		i++;
	}
	return (i);
}

int from_text_at_num(const char *s, int count)
{
	int i = 0;

	while (s[i] >= '0' && s[i] <= '9')
	{
		count *= 10;
		count += s[i] - '0';
		i++;
	}
	return (count);
}

static int count_x(unsigned int x_num, int i)
{
	if (x_num == 0)
		i++;
	while (x_num != 0)
	{
		x_num /= 16;
		i++;
	}
	return (i);
}

static void check_width(t_data *data, const char *s)
{
	if(data->is_width)
	{
		if (s[data->count + 1] == 'd')
		{
			while (data->count_width > count_num(data->num))
			{
				ft_putchar(' ');
				data->count_width--;
			}
		}
		else if (s[data->count + 1] == 's')
		{
			while (data->count_width > ft_strlen(data->str) && !data->is_precison)
			{
				ft_putchar(' ');
				data->count_width--;
			}
			while (data->count_width > data->count_precison && data->is_precison)
			{
				ft_putchar(' ');
				data->count_width--;
			}
		}
		else if (s[data->count + 1] == 'x')
		{
			while (data->count_width > count_x(data->x_int, 0) ||
			data->count_precison > count_x(data->x_int, 0));
			{
				if (data->count_width > data->count_precison)
					ft_putchar(' ');
				else
					ft_putchar('0');
				data->count_width--;
			}
		}
	}
}

static void check_precison(t_data *data, const char *s)
{
	if (data->is_precison)
	{
		while (data->count_precison > count_num(data->num))
		{
			ft_putchar(' ');
			data->count_precison--;
		}
	}
	else if (s[data->count + 1] == 's')
	{
		while (data->count_precison > ft_strlen(data->str))
		{
			ft_putchar(' ');
			data->count_precison--;
		}
	}
	else if (s[data->count + 1] == 'x')
		{
			while (data->count_precison > count_x(data->x_int, 0))
			{
				ft_putchar(' ');
				data->count_precison--;
			}
		}
}

static void change_str(t_data *data)
{
	int i;

	i = 0;
	if(data->is_precison)
	{
		while (data->str[i] != '\0' && i < data->count_precison)
		{
			ft_putchar(data->str[i]);
			i++;
		}
	}
	else
		ft_putstr(data->str);
}

static void change_ss(t_data *data, unsigned int x_num)
{
	if (x_num != 0)
	{
		change_ss(data, x_num / 16);
		if(x_num % 16 > 9)
			ft_putchar(x_num % 16 + 'a' - 10);
		else
			ft_putchar(x_num % 16 + '0');
	}
}

static void check_args(t_data *data, const char *s)
{
	if (s[data->is_procent] == '%' && s[data->count + 1] == 'd')
	{
		data->num = va_arg(data->args, int);
		check_width(data, s);
		ft_putchar_num(data->num);
		check_precison(data, s);
		data->count += 2;
	}
	else if (s[data->is_procent] == '%' && s[data->count + 1] == 's')
	{//точность показывает сколько символов выводить
		data->str = va_arg(data->args, char *);
		check_width(data, s);
		change_str(data);
		check_precison(data, s);
		data->count += 2;
	}
	else if (s[data->is_procent] == '%' && s[data->count + 1] == 'x')
	{
		data->x_int = va_arg(data->args, unsigned int);
		check_width(data, s);
		change_ss(data, data->x_int);
		data->count += 2;
	}
	else
	{
		ft_putchar(s[data->count]);
		data->count++;
	}
}

void check_flags(const char *s, t_data *data)
{
	data->is_width = 0;
	data->is_precison = 0;
	if (s[data->count] == '%')
	{
		data->is_procent = data->count;
		if (is_num(s[data->count + 1]))
		{
			data->is_width = 1;
			data->count_width = from_text_at_num(&s[data->count + 1], 0);
			data->count += count_num(data->count_width);
		}
		if (s[data->count + 1] == '.')
		{
			data->count++;
			data->is_precison = 1;
			data->count_precison = from_text_at_num(&s[data->count + 1], 0);
			data->count += count_num(data->count_precison);
		}
	}
}

int ft_printf(const char *s, ...)
{
	t_data data;

	data.count = 0;
	va_start(data.args, s);
	while (s[data.count] != '\0')
	{
		check_flags(s, &data);
		check_args(&data, s);
	}
}

int main()
{
	// printf("ab%10.11xe\n", 12345);
	ft_printf("%10.11xe\n", 12345);//s d x
}
