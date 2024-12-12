/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:12:21 by jesuserr          #+#    #+#             */
/*   Updated: 2024/12/12 20:23:55 by jesuserr         ###   ########.fr       */
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
	t_hash_args	args;
	t_enc_args	enc_args;

	if (pre_parser(argc, argv) == HASH_COMMAND)
	{
		ft_bzero(&args, sizeof(t_hash_args));
		parse_hash_arguments(argc, argv, &args);
		calls_to_hashing_function(&args);
	}
	else if (pre_parser(argc, argv) == ENCODE_COMMAND)
	{
		ft_bzero(&enc_args, sizeof(t_enc_args));
		printf("Cypher command\n");
	}
	else
		print_error_and_exit("Wrong Hash/Cypher command");
	return (EXIT_SUCCESS);
}
