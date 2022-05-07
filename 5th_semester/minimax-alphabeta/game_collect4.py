import math
import random
import numpy as np


class GameCollect4:
    def __init__(self):
        self.rows = 6
        self.cols = 7
        self.board = np.zeros((self.rows, self.cols))
        self.disc_Player = 1
        self.disc_AI = 2
        self.turn_Player = 0
        self.turn_AI = 1

    def print_board(self):
        state = np.flip(self.board, 0)
        print("\n")
        for row in state:
            for disc in row:
                if disc == 0:
                    print("-", end='  ')
                if disc == 1:
                    print("O", end='  ')
                if disc == 2:
                    print("X", end='  ')
            print("\n")
        for i in range(1, 8):
            print(i, end='  ')
        print("\n ")
        return

    @staticmethod
    def place_disc(board, row, col, disc):
        board[row][col] = disc
        return

    def get_empty_place(self, board_, col):
        for row in range(self.rows):
            if board_[row][col] == 0:
                return row

    def is_correct_move(self, board_, col):
        if col < 0 or col > 6:
            return False
        if board_[self.rows - 1][col] == 0:
            return True
        else:
            return False

    def get_correct_moves(self, board_):
        correct_moves = []
        for column in range(self.cols):
            if self.is_correct_move(board_, column):
                correct_moves.append(column)
        return correct_moves

    def winning_move(self, board_, disc):
        for c in range(self.cols - 3):
            for r in range(3, self.rows):
                if board_[r][c] == disc and board_[r - 1][c + 1] == disc and \
                        board_[r - 2][c + 2] == disc and board_[r - 3][c + 3] == disc:
                    return True

        for c in range(self.cols - 3):
            for r in range(self.rows):
                if board_[r][c] == disc and board_[r][c + 1] == disc and board_[r][c + 2] == disc \
                        and board_[r][c + 3] == disc:
                    return True

        for c in range(self.cols - 3):
            for r in range(self.rows - 3):
                if board_[r][c] == disc and board_[r + 1][c + 1] == disc and \
                        board_[r + 2][c + 2] == disc and board_[r + 3][c + 3] == disc:
                    return True

        for c in range(self.cols):
            for r in range(self.rows - 3):
                if board_[r][c] == disc and board_[r + 1][c] == disc and board_[r + 2][c] == disc and \
                        board_[r + 3][c] == disc:
                    return True

    def evaluation(self, window, disc):
        score = 0
        opp_disc = self.disc_Player

        if disc == self.disc_Player:
            opp_disc = self.disc_AI

        if window.count(disc) == 4:
            score += 99
        elif window.count(disc) == 3 and window.count(0) == 1:
            score += 5
        elif window.count(disc) == 2 and window.count(0) == 2:
            score += 2

        if window.count(opp_disc) == 3 and window.count(0) == 1:
            score -= 4

        return score

    def get_score(self, board_, disc):
        score = 0
        arr = []
        arr1 = []
        arr2 = []

        for i in list(board_[:, self.cols // 2]):
            arr.append(int(i))

        a = arr.count(disc)
        score += a * 3

        for row in range(self.rows):
            for i in list(board_[row, :]):
                arr1.append(int(i))
            for c in range(self.cols - 3):
                window = arr1[c:c + 4]
                score += self.evaluation(window, disc)

        for c in range(self.cols):
            for i in list(board_[:, c]):
                arr2.append(int(i))
            for row in range(self.rows - 3):
                window = arr2[row:row + 4]
                score += self.evaluation(window, disc)

        for row in range(self.rows - 3):
            for c in range(self.cols - 3):
                window = [board_[row + i][c + i] for i in range(4)]
                score += self.evaluation(window, disc)

        for row in range(self.rows - 3):
            for c in range(self.cols - 3):
                window = [board_[row + 3 - i][c + i] for i in range(4)]
                score += self.evaluation(window, disc)

        return score

    def minimax(self, board_, alpha, beta, maximizingPlayer, depth):
        correct_moves = self.get_correct_moves(board_)

        is_leaf = (len(self.get_correct_moves(board_)) == 0) or self.winning_move(board_, self.disc_AI)\
                  or self.winning_move(board_, self.disc_Player)
        if depth == 0 or is_leaf:
            if is_leaf:
                if self.winning_move(board_, self.disc_AI):
                    return None, math.inf
                elif self.winning_move(board_, self.disc_Player):
                    return None, -math.inf
                else:
                    return None, 0
            else:
                return None, self.get_score(board_, self.disc_AI)
        if maximizingPlayer:
            num = -math.inf
            column = random.choice(correct_moves)
            for col in correct_moves:
                row = self.get_empty_place(board_, col)
                board_copy = board_.copy()
                self.place_disc(board_copy, row, col, self.disc_AI)
                new_score = self.minimax(board_copy, alpha, beta, False, depth - 1)[1]
                if new_score > num:
                    num = new_score
                    column = col
                alpha = max(alpha, num)
                if alpha >= beta:
                    break
            return column, num

        else:
            num = math.inf
            column = random.choice(correct_moves)
            for col in correct_moves:
                row = self.get_empty_place(board_, col)
                board_copy = board_.copy()
                self.place_disc(board_copy, row, col, self.disc_Player)
                new_score = self.minimax(board_copy, alpha, beta, True, depth - 1)[1]
                if new_score < num:
                    num = new_score
                    column = col
                beta = min(beta, num)
                if alpha >= beta:
                    break
            return column, num

    def play_game(self):
        self.print_board()
        game_over = False
        game_turn = random.randint(self.turn_Player, self.turn_AI)
        while True:
            if game_turn == self.turn_Player:
                print("Player's turn")
                col = int(input("Enter the number of column {1,7} : ")) - 1

                if self.is_correct_move(self.board, col):
                    row = self.get_empty_place(self.board, col)
                    self.place_disc(self.board, row, col, self.disc_Player)
                    game_turn += 1
                    game_turn = game_turn % 2
                    self.print_board()
                    if self.winning_move(self.board, self.disc_Player):
                        game_over = True
                        print("Congratulations you win! \n")
                else:
                    print("Invalid column number \n")
                    game_turn = self.turn_Player

            elif game_turn == self.turn_AI and not game_over:
                col, minimax_score = self.minimax(self.board, -math.inf, math.inf, True, 6)
                if self.is_correct_move(self.board, col):
                    row = self.get_empty_place(self.board, col)
                    self.place_disc(self.board, row, col, self.disc_AI)
                    print("Program turn")
                    self.print_board()
                    game_turn += 1
                    game_turn = game_turn % 2
                    if self.winning_move(self.board, self.disc_AI):
                        game_over = True
                        print("Program has won! :( \n")
            if game_over:
                print("Play another game! :) \n\n")
                return


collect4 = GameCollect4()
collect4.play_game()
