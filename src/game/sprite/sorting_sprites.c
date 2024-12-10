/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sorting_sprites.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npigeon <npigeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 12:08:13 by npigeon           #+#    #+#             */
/*   Updated: 2024/12/10 12:08:13 by npigeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	sort_sprites(t_game *game, t_sprite **head)
{
	t_sprite	*left;
	t_sprite	*right;

	if (!head || !*head || !(*head)->next)
		return ;
	split_list(*head, &left, &right);
	sort_sprites(game, &left);
	sort_sprites(game, &right);
	*head = merge_sorted_lists(game, left, right);
}

void	sort_sprites_swap(t_sprite **prev, t_sprite **current, t_sprite **next,
int *swapped)
{
	t_sprite	*tmp;

	if (*prev)
		(*prev)->next = *next;
	tmp = *next;
	(*current)->next = (*next)->next;
	(*next)->next = *current;
	*next = *current;
	*current = tmp;
	*swapped = true;
}

void	sort_sprites_not_swap(t_sprite **prev, t_sprite **current,
t_sprite **next)
{
	*prev = *current;
	*current = (*current)->next;
	*next = (*next)->next;
}

t_sprite	*merge_sorted_lists(t_game *game, t_sprite *left,
t_sprite *right)
{
	t_sprite	*result;

	if (!left)
		return (right);
	if (!right)
		return (left);
	if (calculate_distance(game, left) >= calculate_distance(game, right))
	{
		result = left;
		result->next = merge_sorted_lists(game, left->next, right);
	}
	else
	{
		result = right;
		result->next = merge_sorted_lists(game, left, right->next);
	}
	return (result);
}

void	split_list(t_sprite *source, t_sprite **left, t_sprite **right)
{
	t_sprite	*fast;
	t_sprite	*slow;

	slow = source;
	fast = source->next;
	while (fast)
	{
		fast = fast->next;
		if (fast)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}
	*left = source;
	*right = slow->next;
	slow->next = NULL;
}
