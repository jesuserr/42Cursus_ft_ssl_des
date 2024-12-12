/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 22:21:30 by jesuserr          #+#    #+#             */
/*   Updated: 2024/12/12 20:24:02 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/ft_ssl.h"

// Prints given array of bytes in hexadecimal format. Depending on the 'start'
// and 'end' values, it prints the array in ascending or descending order to
// deal with different digest formats (big-endian / little-endian).
// 48 is ASCII code for '0', 87 is ASCII code for 'a'
void	print_hex_bytes(uint8_t *byte, uint8_t start, uint8_t end)
{
	bool	increase;

	increase = start < end;
	while (1)
	{
		if ((byte[start] >> 4) < 10)
			ft_printf("%c", (byte[start] >> 4) + 48);
		else
			ft_printf("%c", (byte[start] >> 4) + 87);
		if ((byte[start] & 0x0F) < 10)
			ft_printf("%c", (byte[start] & 0x0F) + 48);
		else
			ft_printf("%c", (byte[start] & 0x0F) + 87);
		if (start == end)
			break ;
		if (increase)
			start++;
		else
			start--;
	}
}

void	print_error_and_exit(char *str)
{
	ft_printf("ft_ssl: usage error: %s\n", str);
	ft_printf("Try 'ft_ssl -h' for more information.\n");
	exit (EXIT_FAILURE);
}

void	print_total_usage(void)
{
	ft_printf("Usage\n"
		"  ./ft_ssl <command> [flags] [file]\n\n"
		"Hash options:\n"
		"  command     md5, sha224, sha256, sha384 or sha512\n"
		"  -h          print help and exit\n"
		"  -p          echo STDIN to STDOUT and append the checksum to STDOUT\n"
		"  -q          quiet mode\n"
		"  -r          reverse the format of the output\n"
		"  -s <string> print the sum of the given string\n\n"
		"Cypher options:\n"
		"  command     base64\n"
		"  -h          print help and exit\n"
		"  -d          decode mode\n"
		"  -e          encode mode (default)\n"
		"  -i <file>   input file\n"
		"  -o <file>   output file\n");
	exit(EXIT_SUCCESS);
}
