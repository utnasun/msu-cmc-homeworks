import random
import typing as tp
import pygame

Cell = tp.Tuple[int, int]
Cells = tp.List[int]
Grid = tp.List[Cells]


class GameOfLife:
    def __init__(
            self,
            size: tp.Tuple[int, int],
            cell_size: int = 20,
            randomize: bool = True,
            max_generations: tp.Optional[float] = float("inf"),
            speed: int = 5
    ) -> None:
        # Размер клеточного поля
        self.rows, self.cols = size
        # Предыдущее поколение клеток
        self.prev_generation = self.create_grid()
        # Текущее поколение клеток
        self.curr_generation = self.create_grid(randomize=randomize)
        # Максимальное число поколений
        self.max_generations = max_generations
        # Текущее число поколений
        self.n_generation = 1
        self.cell_size = cell_size
        self.screen_size = self.cols * cell_size, self.rows * cell_size
        self.screen = pygame.display.set_mode(self.screen_size)
        self.speed = speed

    def create_grid(self, randomize: bool = False) -> Grid:
        grid_matrix = []
        if randomize:
            for i in range(self.rows):
                append_list = []
                for j in range(self.cols):
                    append_list.append(random.randint(0, 2))
                grid_matrix.append(append_list)
        else:
            grid_matrix = [[0] * self.cols for _ in range(self.rows)]
        return grid_matrix

    def get_neighbours(self, cell: Cell) -> Cells:
        neighbors = []
        relatives_util = [[-1, -1], [1, 1], [0, 1], [1, 0], [-1, 1], [1, -1], [-1, 0], [0, -1]]
        for el in relatives_util:
            el[0] += cell[0]
            el[1] += cell[1]
        for el in relatives_util:
            i, j = el
            try:
                if i >= 0 and j >= 0:
                    neighbors.append(self.curr_generation[i][j])
            except IndexError:
                pass
        return neighbors

    def get_next_generation(self) -> Grid:
        new_greed = self.create_grid(False)
        for i in range(self.rows):
            for j in range(self.cols):
                if self.curr_generation[i][j]:
                    if sum(self.get_neighbours((i, j))) in [2, 3]:
                        new_greed[i][j] = random.randint(1, 2)
                elif sum(self.get_neighbours((i, j))) == 3:
                    new_greed[i][j] = random.randint(1, 2)
        return new_greed

    def step(self) -> None:
        self.prev_generation = self.curr_generation
        self.curr_generation = self.get_next_generation()
        self.n_generation += 1

    @property
    def is_max_generations_exceed(self) -> bool:
        if self.n_generation > self.max_generations:
            return False
        else:
            return True

    def draw_lines(self) -> None:
        for x in range(0, self.cols):
            pygame.draw.line(self.screen, pygame.Color("black"), (x * self.cell_size, 0),
                             (x * self.cell_size, self.rows * self.cell_size))
        for y in range(0, self.rows):
            pygame.draw.line(self.screen, pygame.Color("black"), (0, y * self.cell_size),
                             (self.cols * self.cell_size, y * self.cell_size))

    def draw_grid(self) -> None:
        y = 0
        for row in self.curr_generation:
            x = 0
            for el in row:
                if el == 2:
                    color = 'red'
                elif el == 1:
                    color = 'green'
                else:
                    color = 'white'
                pygame.draw.rect(self.screen, pygame.Color(color),
                                 pygame.Rect(x, y, self.cell_size, self.cell_size))
                x += self.cell_size
            y += self.cell_size

    def colorize_cell(self, click_coords: (int, int)):
        j, i = click_coords[0] // self.cell_size, click_coords[1] // self.cell_size
        if self.curr_generation[i][j]:
            self.curr_generation[i][j] = 0
        else:
            self.curr_generation[i][j] = random.randint(1, 2)

    def run(self) -> None:
        """ Запустить игру """
        pygame.init()
        clock = pygame.time.Clock()
        pygame.display.set_caption("Game of Life")
        self.screen.fill(pygame.Color("white"))

        # Создание списка клеток
        # PUT YOUR CODE HERE

        running = True
        playing = False
        self.draw_grid()
        self.draw_lines()
        pygame.display.flip()
        while running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
                if event.type == pygame.MOUSEBUTTONUP and event.button == 1 and not playing:
                    self.colorize_cell(pygame.mouse.get_pos())
                    self.draw_grid()
                    self.draw_lines()
                    pygame.display.flip()
                if event.type == pygame.MOUSEBUTTONUP and event.button == 3:
                    playing = not playing
            if playing:
                self.step()
                self.draw_grid()
                self.draw_lines()
                pygame.display.flip()
                clock.tick(self.speed)
        pygame.quit()


life = GameOfLife((24, 80), randomize=True, max_generations=50)
life.run()
