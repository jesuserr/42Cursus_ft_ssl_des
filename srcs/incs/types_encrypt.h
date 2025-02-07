/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types_encrypt.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:48:16 by jesuserr          #+#    #+#             */
/*   Updated: 2025/02/07 21:15:05 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_ENCRYPT_H
# define TYPES_ENCRYPT_H

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              DEFINES
*/
# define ITERATIONS 		10000U		// Number of iterations for PBKDF2
# define BLOCK_LENGTH		8U			// Block length in bytes (64 bits)
										// Applies to main key, salt and IV
# define SUBKEY_LENGTH		7U			// Subkey length in bytes (56 bits)
# define ROUND_KEY_LENGTH	6U			// Round keys length in bytes (48 bits)
# define SALT_STR			"Salted__"	// Salt string for DES encryption
# define SALT_LENGTH		8U			// Salt length in bytes (64 bits)
# define SALT_TOTAL_LEN		16U			// (Salt string + Salt) length in bytes
# define ROUNDS				16U			// Number of rounds for DES encryption

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              STRUCTS
*/
typedef struct s_encrypt_args
{
	char		*input_pipe;
	char		*input_file;
	char		*message;
	char		*input_file_name;
	uint64_t	input_file_size;
	uint64_t	pipe_size;
	uint64_t	message_length;
	int			output_fd;
	bool		decrypt_mode;
	bool		encrypt_mode;
	bool		input_from_file;
	bool		output_to_file;
	bool		base64_mode;
	bool		key_provided;
	bool		pass_provided;
	bool		salt_provided;
	bool		iv_provided;
	char		*key;
	char		*pass;
	char		*salt;
	char		*iv;
	uint8_t		encrypt_function;
	uint8_t		hex_key[BLOCK_LENGTH];
	uint8_t		hex_iv[BLOCK_LENGTH];
	uint8_t		hex_salt[BLOCK_LENGTH];
	uint8_t		subkeys[ROUNDS][ROUND_KEY_LENGTH];
}	t_encrypt_args;

// Permutation tables for DES encryption. Example: for 'g_pc1_table' the first
// bit of the output is taken from the 57th bit of the input; the second bit
// from the 49th bit, and so on, with the last bit of the output taken from the
// 4th bit of the input.
// Tables for subkeys generation
static const uint8_t	g_pc1_table[56] = {
	57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35,
	27,	19, 11, 3, 60, 52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46,
	38, 30, 22,	14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4};

static const uint8_t	g_pc2_table[48] = {
	14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8,
	16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32};

static const uint8_t	g_shift_table[16] = {
	1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

// Tables for initial and final permutation
static const uint8_t	g_ip_table[64] = {
	58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4, 62, 54,
	46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8, 57, 49, 41, 33,
	25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3, 61, 53, 45, 37, 29, 21,
	13, 5, 63, 55, 47, 39, 31, 23, 15, 7};

static const uint8_t	g_fp_table[64] = {
	40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31, 38, 6,
	46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12,
	52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10, 50, 18,
	58, 26, 33, 1, 41, 9, 49, 17, 57, 25};

#endif
