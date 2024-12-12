/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:12:21 by jesuserr          #+#    #+#             */
/*   Updated: 2024/12/09 19:00:56 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/ft_ssl.h"

// Uses an array of function pointers to call the hashing function selected by
// the user. Performs up to three calls to the hashing function, one for each
// possible input source (pipe, string or file) that has some content.
static void	calls_to_hashing_function(t_arguments *args)
{
	void		(*hash_functions[])(t_arguments *) = \
				{md5_sum, sha224_sum, sha256_sum, sha384_sum, sha512_sum};
	char		*msg[3];
	uint8_t		origin;

	msg[IS_PIPE] = args->input_pipe;
	msg[IS_STRING] = args->input_str;
	msg[IS_FILE] = args->input_file;
	origin = IS_PIPE;
	while (origin <= IS_FILE)
	{
		if (msg[origin])
		{
			args->msg_origin = origin;
			args->message = msg[origin];
			hash_functions[args->hash_function](args);
		}
		origin++;
	}
	if (args->input_pipe)
		free(args->input_pipe);
	if (args->input_file && args->file_size > 0)
		if (munmap(args->input_file, args->file_size) < 0)
			print_strerror_and_exit("munmap", args);
}

int	main(int argc, char **argv)
{
	t_arguments	args;

	if (argc < 2)
		print_error_and_exit("Hash function required");
	ft_bzero(&args, sizeof(t_arguments));
	parse_arguments(argc, argv, &args);
	calls_to_hashing_function(&args);
	return (EXIT_SUCCESS);
}
