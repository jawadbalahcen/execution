/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_stuff_utills.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 00:40:55 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/25 15:39:16 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	disable_sigquit(struct termios *term)
{
	struct termios	new;

	new = *term;
	new.c_cc[VQUIT] = _POSIX_VDISABLE;
	tcsetattr(0, TCSANOW, &new);
}
