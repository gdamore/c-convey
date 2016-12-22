#include "convey.h"

/*
 * This is a hypothetical bowling game.  In practice your test would
 * not have your implementation here, but would probably in some other
 * file.  For demonstration purposes its included here.
 */
struct frame {
	int pins;
	int strike;
	int spare;
};
struct game {
	struct frame frames[12];	/* includes up to 2 bonus frames */
	int throws;
};

void
newGame(struct game *game)
{
	for (int i = 0; i < 12; i++) {
		game->frames[i].pins = 0;
		game->frames[i].strike = 0;
		game->frames[i].spare = 0;
	}
	game->throws = 0;
}

int
score(struct game *game)
{
	int score = 0;
	for (int i = 0; i < 10; i++) {
		score += game->frames[i].pins;
		if (game->frames[i].strike) {
			score += game->frames[i+1].pins;
			score += game->frames[i+2].pins;
		} else if (game->frames[i].spare) {
			score += game->frames[i+1].pins;
		}
	}
	return score;
}

int
gameOver(struct game *game)
{
	int maxthrows = 10;
	if (game->frames[9].strike) {
		maxthrows = 12;
	} else if (game->frames[9].spare || game->frames[8].strike) {
		maxthrows = 11;
	}
	return ((game->throws < maxthrows) ? 0 : 1);
}

/* roll only counts the full numeric score */
int
roll(struct game *game, int pins)
{
	/* Note: special checks for valid bonus frame values not done. */
	if ((pins > 10) || (pins < 0) || gameOver(game)) {
		return (-1);
	}
	game->frames[game->throws++].pins = pins;
	return (0);
}

int
rollStrike(struct game *game)
{
	if (gameOver(game)) {
		return (-1);
	}
	game->frames[game->throws].strike = 1;
	game->frames[game->throws++].pins = 10;
	return (0);
}

int
rollSpare(struct game *game)
{
	if (gameOver(game)) {
		return (-1);
	}
	game->frames[game->throws].spare = 1;
	game->frames[game->throws++].pins = 10;
	return (0);
}

int
rollMany(struct game *game, int pins, int count)
{
	for (int i = 0; i < count; i++) {
		if (roll(game, pins) != 0) {
			return (-1);
		}
	}
	return (0);
}

/*
 * This is where the test code starts.
 */

Main({
	Test("Game Rules", {
		Convey("Given a fresh score card", {
			struct game game;
			newGame(&game);

			Convey("We cannot roll a negative number", {
				So(roll(&game, -1) == -1);
			});
			Convey("We cannot roll a large number", {
				So(roll(&game, 11) == -1);
			});
			Convey("We can roll in the middle", {
				So(roll(&game, 9) == 0);
			});
			Convey("We can roll only 10 frames normally", {
				for (int i = 0; i < 10; i++) {
					So(roll(&game, 4) == 0);
				}
				So(roll(&game, 4) == -1);
			});
			Convey("We can roll bonus frame if tenth was spare", {
				for (int i = 0; i < 10; i++) {
					So(rollSpare(&game) == 0);
				}
				So(roll(&game, 4) == 0);
				So(roll(&game, 4) == -1); /* only one bonus */
			});

		});
	});
	Test("Game Scoring", {
		struct game game;
		Convey("Given a fresh score card", {
			newGame(&game);

			Convey("When all gutter balls are thrown", {
				rollMany(&game, 10, 0);

				Convey("The score should be zero", {
					So(score(&game) == 0);
				})
			});

			Convey("When all throws knock down one pin", {
				rollMany(&game, 10, 1);

				Convey("The score should be 10", {
					So(score(&game) == 10);
				})
			});

			Convey("When a spare is thrown", {
				rollSpare(&game);
				roll(&game, 3);

				Convey("The score includes a spare bonus", {
					So(score(&game) == 16);
				});
			});

			Convey("A perfect game is 300 points", {
				for (int i = 0; i < 12; i++) {
					rollStrike(&game);
				}
				So(score(&game) == 300);
			});
		});
	});
})
