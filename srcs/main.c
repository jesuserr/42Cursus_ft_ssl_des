/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:12:21 by jesuserr          #+#    #+#             */
/*   Updated: 2025/01/04 14:12:35 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/ft_ssl.h"

static uint8_t	pre_parser(int argc, char **argv)
{
	if (argc < 2)
		print_error_and_exit("Hash/Cypher function required");
	if ((!ft_strncmp(argv[1], "md5", 3) && ft_strlen(argv[1]) == 3) || \
		(!ft_strncmp(argv[1], "sha224", 6) && ft_strlen(argv[1]) == 6) || \
		(!ft_strncmp(argv[1], "sha256", 6) && ft_strlen(argv[1]) == 6) || \
		(!ft_strncmp(argv[1], "sha384", 6) && ft_strlen(argv[1]) == 6) || \
		(!ft_strncmp(argv[1], "sha512", 6) && ft_strlen(argv[1]) == 6))
		return (HASH_COMMAND);
	else if (!ft_strncmp(argv[1], "base64", 6) && ft_strlen(argv[1]) == 6)
		return (ENCODE_COMMAND);
	else if (!ft_strncmp(argv[1], "-h", 2) && ft_strlen(argv[1]) == 2)
		print_total_usage();
	return (0);
}

int	main(int argc, char **argv)
{
	t_hash_args		hash_args;
	t_encode_args	encode_args;

	if (pre_parser(argc, argv) == HASH_COMMAND)
	{
		ft_bzero(&hash_args, sizeof(t_hash_args));
		parse_hash_arguments(argc, argv, &hash_args);
		calls_to_hashing_function(&hash_args);
	}
	else if (pre_parser(argc, argv) == ENCODE_COMMAND)
	{
		ft_bzero(&encode_args, sizeof(t_encode_args));
		parse_encode_arguments(argc, argv, &encode_args);
		calls_to_decoding_function(&encode_args);
	}
	else
		print_error_and_exit("Wrong Hash/Cipher command");
	return (EXIT_SUCCESS);
}
