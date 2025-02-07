/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encrypt_block_cipher.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 19:39:39 by jesuserr          #+#    #+#             */
/*   Updated: 2025/02/07 21:22:25 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_ssl.h"

// Permutate the message provided in 'src' and save it into 'dst' using the
// given permutation table and its length.
// Not tested for expansion tables yet!!
static void	bitwise_permutation(const uint8_t *src, uint8_t *dst, \
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
static void	rotate_each_half_key(uint8_t *key, uint8_t rot)
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
	uint8_t	pc_1_keys[SUBKEY_LENGTH];
	uint8_t	i;

	ft_bzero(pc_1_keys, sizeof(pc_1_keys));
	bitwise_permutation(args->hex_key, pc_1_keys, g_pc1_table, \
	sizeof(g_pc1_table));
	i = 0;
	while (i < ROUNDS)
	{
		rotate_each_half_key(pc_1_keys, g_shift_table[i]);
		bitwise_permutation(pc_1_keys, args->subkeys[i], g_pc2_table, \
		sizeof(g_pc2_table));
		i++;
	}
	//ft_printf("Subkeys: \n");
	//ft_hex_dump(args->subkeys, ROUNDS * ROUND_KEY_LENGTH, ROUND_KEY_LENGTH);
}
