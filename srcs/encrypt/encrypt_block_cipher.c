/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encrypt_block_cipher.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 19:39:39 by jesuserr          #+#    #+#             */
/*   Updated: 2025/02/08 17:10:56 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_ssl.h"

// Permutate the bits of the message provided in 'src' and save it into 'dst' 
// using the given permutation table and its length.
// 'src' and 'dst' cannot be the same.
void	bitwise_permutation(const uint8_t *src, uint8_t *dst, \
		const uint8_t *table, uint8_t length)
{
	uint8_t	i;
	uint8_t	bit;

	i = 0;
	while (i < length)
	{
		bit = (src[(table[i] - 1) / 8] >> (7 - ((table[i] - 1) % 8))) & 0x01;
		dst[i / 8] |= (bit << (7 - i % 8));
		i++;
	}
}

// Rotates independently to the left the first 28 bits and the last 28 bits 
// of the key. Number of left rotations are specified in the shift table and 
// passed through 'rot'.
static void	left_rotate_each_half_key(uint8_t *key, uint8_t rot)
{
	uint32_t	first_half;
	uint32_t	second_half;

	first_half = ((uint32_t)key[0] << 20) | ((uint32_t)key[1] << 12) | \
	((uint32_t)key[2] << 4) | ((uint32_t)key[3] >> 4);
	first_half = ((first_half << rot) | (first_half >> (28 - rot))) & \
	0x0FFFFFFF;
	key[0] = (first_half >> 20) & 0xFF;
	key[1] = (first_half >> 12) & 0xFF;
	key[2] = (first_half >> 4) & 0xFF;
	key[3] = (key[3] & 0x0F) | ((first_half & 0x0F) << 4);
	second_half = ((uint32_t)(key[3] & 0x0F) << 24) | ((uint32_t)key[4] << 16) \
	| ((uint32_t)key[5] << 8) | ((uint32_t)key[6]);
	second_half = ((second_half << rot) | (second_half >> (28 - rot))) & \
	0x0FFFFFFF;
	key[3] = (key[3] & 0xF0) | ((second_half >> 24) & 0x0F);
	key[4] = (second_half >> 16) & 0xFF;
	key[5] = (second_half >> 8) & 0xFF;
	key[6] = second_half & 0xFF;
}

// Generates the 16 subkeys for the DES encryption algorithm. The key is
// permuted using the PC-1 table, then rotated and permuted again ROUNDS times
// using the PC-2 table. The result is stored in 'args->subkeys'.
void	generate_subkeys(t_encrypt_args *args)
{
	uint8_t	pc_1_key[SUBKEY_LENGTH];
	uint8_t	i;

	ft_bzero(pc_1_key, sizeof(pc_1_key));
	bitwise_permutation(args->hex_key, pc_1_key, g_pc1_table, \
	sizeof(g_pc1_table));
	i = 0;
	while (i < ROUNDS)
	{
		left_rotate_each_half_key(pc_1_key, g_shift_table[i]);
		bitwise_permutation(pc_1_key, args->subkeys[i], g_pc2_table, \
		sizeof(g_pc2_table));
		i++;
	}
}

// Performs the keyed substitution step of the mangler function using S-boxes.
// Input is a 6-byte array and output is a 4-byte array.
static void	keyed_substitution(uint8_t *input, uint8_t *output)
{
	uint32_t	temp;

	temp = 0;
	temp |= ((g_s_boxes[0][((input[0] & 0x80) >> 6) | ((input[0] & 0x04) >> 2)] \
	[(input[0] & 0x78) >> 3]) & 0x0F) << 28;
	temp |= ((g_s_boxes[1][((input[0] & 0x02) >> 0) | ((input[1] & 0x10) >> 4)] \
	[((input[0] & 0x01) << 3) | ((input[1] & 0xE0) >> 5)]) & 0x0F) << 24;
	temp |= ((g_s_boxes[2][((input[1] & 0x08) >> 2) | ((input[2] & 0x40) >> 6)] \
	[((input[1] & 0x07) << 1) | ((input[2] & 0x80) >> 7)]) & 0x0F) << 20;
	temp |= ((g_s_boxes[3][((input[2] & 0x20) >> 4) | ((input[2] & 0x01) >> 0)] \
	[((input[2] & 0x1E) >> 1)]) & 0x0F) << 16;
	temp |= ((g_s_boxes[4][((input[3] & 0x80) >> 6) | ((input[3] & 0x04) >> 2)] \
	[((input[3] & 0x78) >> 3)]) & 0x0F) << 12;
	temp |= ((g_s_boxes[5][((input[3] & 0x02) >> 0) | ((input[4] & 0x10) >> 4)] \
	[((input[3] & 0x01) << 3) | ((input[4] & 0xE0) >> 5)]) & 0x0F) << 8;
	temp |= ((g_s_boxes[6][((input[4] & 0x08) >> 2) | ((input[5] & 0x40) >> 6)] \
	[((input[4] & 0x07) << 1) | ((input[5] & 0x80) >> 7)]) & 0x0F) << 4;
	temp |= ((g_s_boxes[7][((input[5] & 0x20) >> 4) | ((input[5] & 0x01) >> 0)] \
	[((input[5] & 0x1E) >> 1)]) & 0x0F) << 0;
	output[0] = (temp >> 24) & 0xFF;
	output[1] = (temp >> 16) & 0xFF;
	output[2] = (temp >> 8) & 0xFF;
	output[3] = (temp >> 0) & 0xFF;
}

// Performs the mangler function of the DES encryption algorithm. The right half
// of the message is expanded, XORed with the subkey, passed through the S-boxes
// and permuted using the P table. The result is returned back in 'right_half'.
void	mangler(uint8_t *right_half, uint8_t round, t_encrypt_args *args)
{
	uint8_t	expanded_right[ROUND_KEY_LENGTH];
	uint8_t	permuted_right[ROUND_KEY_LENGTH];
	uint8_t	i;

	ft_bzero(expanded_right, sizeof(expanded_right));
	bitwise_permutation(right_half, expanded_right, g_exp_table, \
	sizeof(g_exp_table));
	i = 0;
	while (i < ROUND_KEY_LENGTH)
	{
		expanded_right[i] ^= args->subkeys[round][i];
		i++;
	}
	keyed_substitution(expanded_right, right_half);
	ft_bzero(permuted_right, sizeof(permuted_right));
	bitwise_permutation(right_half, permuted_right, g_p_table, \
	sizeof(g_p_table));
	ft_memcpy(right_half, permuted_right, BLOCK_LENGTH / 2);
}
