#include "funcs.h"

jump_t	take_piece(match_t *match, int xo, int yo, int x, int y, turn_t role) {
	jump_t	prise;
	bool	x_off_positive = true;
	bool	y_off_positive = true;
	
	prise.take = false;
	prise.x = -1;
	prise.y = -1;
	puts("=== Take subpart ===");
	// Check out of bounds
	if (xo > 9 || xo < 0 || x > 9 || x < 0 || y > 9 || y < 0 || yo > 9 || yo < 0)
		return (prise);
	puts("- OOB check passed");
	// Check if landing case is free
	if (match->board[x][y] != 0) {
		prise.illegal = true;
		return (prise);
	}
	puts("- Free case test based");
	// At least 2 diagonal move for dame, only 1 for pawn
	if ((abs(x - xo) != abs(y - yo)) && ((abs(x - xo) < 2 && abs(match->board[xo][yo]) == 2) || (abs(match->board[xo][yo]) == 2 && abs(x - xo) != 2)))
		return (prise);
	puts("- Max move test passed");
	// Set quadrant offsets to setup tests
	if (xo > x)
		x_off_positive = false;
	if (yo > y)
		y_off_positive = false;
	// Return piece coordinates if take is possible depending of the quadrant
	if (x_off_positive == true && y_off_positive == true) {
		for (int i = 1; i < abs(x - xo); i++) {
			if ((match->board[xo + i][yo + i] < 0 && role == GUEST) || (match->board[xo + i][yo + i] > 0 && role == HOST)) {
				if (prise.take == true) {
					prise.illegal = true;
					return (prise);
				}
				prise.take = true;
				prise.x = xo + i;
				prise.y = yo + i;
			}
		}
	}
	if (x_off_positive == false && y_off_positive == true) {
		for (int i = 1; i < abs(x - xo); i++) {
			if ((match->board[xo - i][yo + i] < 0 && role == GUEST) || (match->board[xo - i][yo + i] > 0 && role == HOST)) {
				if (prise.take == true) {
					prise.illegal = true;
					return (prise);
				}
				prise.take = true;
				prise.x = xo - i;
				prise.y = yo + i;
			}
		}
	}
	if (x_off_positive == true && y_off_positive == false) {
		for (int i = 1; i < abs(x - xo); i++) {
			if ((match->board[xo + i][yo - i] < 0 && role == GUEST) || (match->board[xo + i][yo - i] > 0 && role == HOST)) {
				if (prise.take == true) {
					prise.illegal = true;
					return (prise);
				}
				prise.take = true;
				prise.x = xo + i;
				prise.y = yo - i;
			}
		}
	}
	if (x_off_positive == false && y_off_positive == false) {
		for (int i = 1; i < abs(x - xo); i++) {
			if ((match->board[xo - i][yo - i] < 0 && role == GUEST) || (match->board[xo - i][yo - i] > 0 && role == HOST)) {
				if (prise.take == true) {
					prise.illegal = true;
					return (prise);
				}
				prise.take = true;
				prise.x = xo - i;
				prise.y = yo - i;
			}
		}
	}
	printf("=== Piece finding : %d ===\n", prise.take);
	return (prise);
}

// dame HOST -2
// pion HOST -1
// case vide 0
// pion GUEST 1
// dame GUEST 2
piece_info_t	can_place_piece(match_t *match, int xo, int yo, int x, int y, turn_t role) {
	piece_info_t	info;
	
	puts("=== Checking piece validity ===");
	info.legal = false;
	// Check out of bounds
	if (xo > 9 || xo < 0 || x > 9 || x < 0 || y > 9 || y < 0 || yo > 9 || yo < 0)
		return (info);
	puts("- Piece isn't out of bounds");
	// Check if piece is owned (Host case)
	if (match->board[xo][yo] > -1 && role == HOST)
		return (info);
	// Check if piece is owned (Guest case)
	if (match->board[xo][yo] < 1 && role == GUEST)
		return (info);
	puts("- Piece is owned");
	// Check if the reached case is on the diagonal
	if (abs(x - xo) != abs(y - yo))
		return (info);
	puts("- Move is diagonal");
	// Check if no move selected
	if (abs(x - xo) < 1 || abs(y - yo) < 1)
		return (info);
	puts("- Move is effective");
	
	jump_t	prise = take_piece(match, xo, yo, x, y, role);
	// If pawn and no pawn erased, check if 1 case move
	if (abs(x - xo) > 1 && match->board[x][y] == 0 && abs(match->board[xo][yo]) == 1 && prise.take == false)
		return (info);
	puts("- Move is one case (if pawn and no jump)");
	// Check if dame isn't trying to take multiple pieces
	if (prise.illegal == true)
		return (info);
	puts("- Take is at most 1 pawn and/or landing case is free");
	// Check if moving forward (except if taking piece)
	if (x < xo && role == HOST && abs(match->board[xo][yo]) == 1 && prise.take == false)
		return (info);
	if (x > xo && role == GUEST && abs(match->board[xo][yo]) == 1 && prise.take == false)
		return (info);
	puts("- Move is forward");
	if (prise.take == true)
		match->board[prise.x][prise.y] = 0;
	info.legal = true;
	info.prise = prise;
	match->board[x][y] = match->board[xo][yo];
	match->board[xo][yo] = 0;
	puts("== Piece move succeded ==");
	// Upgrade the pawn if reached the last board line
	if (role == GUEST && y == 0)
		match->board[x][y] = 2 * GUEST;
	if (role == HOST && y == 9)
		match->board[x][y] = 2 * HOST;
	return (info);
}

play_info_t	place_piece(list_t **begin_list, int cid, int xo, int yo, int x, int y) {
	list_t			*list;
	play_info_t		play;
	
	play.status = USER_NOT_PLAYING;
	play.oid = -1;
	if (!begin_list)
		play.status = INTERNAL_SERVER_ERROR;
	else if (clients[cid].status.playing != true)
		play.status = USER_NOT_PLAYING;
	else if (*begin_list) {
		list = *begin_list;
		while (list) {
			// Find the match hosted where you're currently playing
			if (!strcmp(list->match.host, clients[cid].status.host) && (!strcmp(list->match.host, clients[cid].status.username) || !strcmp(list->match.guest, clients[cid].status.username))) {
				if (list->match.guest_found == false) {
					play.status = USER_NOT_PLAYING;
					goto ret;
				}
				// Check if player turn and match current turn matches
				play.role = GUEST;
				if (!strcmp(list->match.host, clients[cid].status.username)) {
					for (int k = 0; k < MAX_CLIENTS; k++) {
						if (!strcmp(list->match.guest, clients[k].status.username))
							play.oid = k;
					}
					play.role = HOST;
				}
				else {
					for (int k = 0; k < MAX_CLIENTS; k++) {
						if (!strcmp(list->match.host, clients[k].status.username))
							play.oid = k;
					}
				}
				if (play.oid == -1) {
					play.status = USER_NOT_PLAYING;
					goto ret;
				}
				if (play.role != list->match.turn) {
					play.status = NOT_YOUR_TURN;
					goto ret;
				}	
				// Place the piece, send an error status if wrong placement
				play.info = can_place_piece(&(list->match), xo, yo, x, y, play.role);
				if (play.info.legal == false) {
					play.status = INVALID_PLACEMENT;
					goto ret;
				}
				
				// Change turn
				list->match.turn *= -1;
				
				// To do : Check win, etc
				// To do : Handle rafles
				// To do : Force prise if possible
				strcpy(play.hostname, list->match.host);
				play.status = SUCCESS;
				goto ret;
			}
			list = list->next;
		}
	}
	ret:
	return (play);
}