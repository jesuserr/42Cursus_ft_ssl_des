/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encode_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 13:03:15 by jesuserr          #+#    #+#             */
/*   Updated: 2024/12/15 18:33:03 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/ft_ssl.h"

static void	parse_options(int opt, t_encode_args *args)
{
	if (opt == 'h')
		print_encode_usage();
	else if (opt == 'd' && !args->decode_mode)
		args->decode_mode = true;
	else if (opt == 'e' && !args->encode_mode)
		args->encode_mode = true;
	else if (opt == 'i' && !args->input_from_file)
	{
		if (!ft_isalnum(optarg[0]))
			print_error_and_exit("No input file specified");
		args->input_from_file = true;
		args->input_file_name = optarg;
	}
	else if (opt == 'o' && !args->output_to_file)
	{
		if (!ft_isalnum(optarg[0]))
			print_error_and_exit("No output file specified");
		args->output_to_file = true;
		args->output_file_name = optarg;
	}
}

// Function deals with both binary and text files. 'isatty' function is used to
// check if the input is coming from a pipe. Message is read in chunks of
// BUFFER_SIZE bytes and with the help of 'realloc' and 'ft_memcpy', the whole
// message is stored in 'input_pipe'. If the file is empty, the program will not
// read anything and the input_pipe will be NULL.
// IMPORTANT: Since input can be binary, the message contained in 'input_pipe' 
// is not null-terminated, and therefore cannot be printed with 'printf'.
static void	parse_pipe(t_encode_args *args)
{
	char		buffer[BUFFER_SIZE];
	char		*temp;
	ssize_t		bytes_read;

	if (isatty(STDIN_FILENO) != 0)
		return ;
	bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		temp = realloc(args->input_pipe, args->pipe_size + (size_t)bytes_read);
		if (!temp)
			print_encode_strerror_and_exit("realloc", args);
		args->input_pipe = temp;
		ft_memcpy(args->input_pipe + args->pipe_size, buffer, \
		(size_t)bytes_read);
		args->pipe_size += (size_t)bytes_read;
		bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
	}
	if (bytes_read < 0)
	{
		free(args->input_pipe);
		print_error_and_exit("Error reading from pipe");
	}
}

// Uses 'mmap' to map the entire file into memory in one shot. Way more 
// efficient than reading the file multiple times. File size is kept for the
// hash functions to know how many bytes to read (specially for binary files)
// and also for the 'munmap' function to know how many bytes to unmap when the
// program finishes. Empty file case is handled too, otherwise 'mmap' would
// fail.
static void	parse_file_content(t_encode_args *args, char *file_name)
{
	int			fd;
	struct stat	file_stat;
	void		*file_content;

	fd = open(file_name, O_RDONLY);
	if (fd < 0)
		print_encode_strerror_and_exit(file_name, args);
	if (fstat(fd, &file_stat) < 0)
		print_encode_strerror_and_exit(file_name, args);
	if (file_stat.st_size > 0)
	{
		file_content = mmap(NULL, (size_t)file_stat.st_size, PROT_READ, \
		MAP_PRIVATE, fd, 0);
		if (file_content == MAP_FAILED)
		{
			close(fd);
			print_encode_strerror_and_exit("mmap", args);
		}
		args->input_file = (char *)file_content;
	}
	else
		args->input_file = "";
	close(fd);
	args->input_file_size = (uint64_t)file_stat.st_size;
	args->input_file_name = file_name;
}

// Parse main function.
void	parse_encode_arguments(int argc, char **argv, t_encode_args *args)
{
	int		opt;

	opt = getopt(argc, argv, "hdei:o:");
	while (opt != -1)
	{
		parse_options(opt, args);
		opt = getopt(argc, argv, "hdei:o:");
	}
	if (args->decode_mode && args->encode_mode)
		print_error_and_exit("Cannot use both -d and -e flags");
	else if (!args->decode_mode && !args->encode_mode)
		args->encode_mode = true;
	parse_pipe(args);
	optind++;
	if (!argv[optind] && !args->input_pipe && !args->input_from_file)
		read_interactive_mode(&args->input_pipe, &args->pipe_size);
	else if (args->input_from_file)
		parse_file_content(args, args->input_file_name);
	if (optind < argc)
		print_error_and_exit("Too many arguments");
}
