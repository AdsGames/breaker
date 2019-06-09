#include "game.h"

#include "globals.h"
#include "utility/tools.h"
#include "utility/MouseListener.h"

// INIT
game::game() {
  // Sets Cursors
  cursor[0] = load_png_ex ("images/cursor1.png");
  cursor[1] = load_png_ex ("images/cursor2.png");

  // Creates a buffer
  buffer = create_bitmap (1280, 960);

  // Sets background
  background = load_png_ex ("images/background.png");

  // Sets Foreground
  foreground = load_png_ex ("images/foreground.png");

  // Sets menu
  dialog_box = load_png_ex ("images/menu.png");

  // Trans overlay
  trans_overlay = load_png_ex ("images/overlay.png");

  // Sets Sounds
  block_break = load_sample_ex ("sounds/break.wav");
  click = load_sample_ex ("sounds/click.wav");

  // Sets Variables
  score = 0;
  startAnimate = 1200;
  blocks_selected = 0;
  game_over = false;
  game_over_message = "Game Over";

  // Sets block info
  for (int i = 0; i < 14; i++) {
    for (int t = 0; t < 9; t++) {
      MyBlocks[i][t] = Block(i * 80 + 80, t * 80 + 80, random (0, difficulty));
    }
  }

  // Give score files
  highscores = ScoreManager ("scores.dat");

  // Buttons
  done = Button (500, 10);
  dialog_yes = Button (41 + 300, 215 + 300);
  dialog_no = Button (461 + 300, 215 + 300);

  // Images
  done.SetImages ("images/buttons/done.png", "images/buttons/done_hover.png");
  dialog_yes.SetImages ("images/buttons/yes.png", "images/buttons/yes_hover.png");
  dialog_no.SetImages ("images/buttons/no.png", "images/buttons/no_hover.png");

  // On clicks
  done.SetOnClick([this]() {
    game_over = true;
  });

  dialog_yes.SetOnClick([this]() {
    highscores.add (ib_name.GetValue(), score);
    set_next_state (STATE_GAME);
  });

  dialog_no.SetOnClick([this]() {
    highscores.add (ib_name.GetValue(), score);
    set_next_state (STATE_MENU);
  });

  ib_name = InputBox(488, 405, 404, 44, "Player");

  particles = particle_emitter(vec2(0, 0), vec2(64, 64));
}

// DESTORY
game::~game() {
  destroy_bitmap (buffer);
  destroy_bitmap (background);
  destroy_bitmap (cursor[0]);
  destroy_bitmap (cursor[1]);
  destroy_bitmap (foreground);
  destroy_bitmap (dialog_box);
  destroy_bitmap (trans_overlay);

  destroy_sample (block_break);
  destroy_sample (click);
}

// Deselect all blocks
void game::deselect_blocks() {
  for (int i = 0; i < BLOCKS_WIDE; i++)
    for (int t = 0; t < BLOCKS_HIGH; t++) {
      MyBlocks[i][t].setSelected (false);
    }
}

// Select group of blocks
int game::select_block (int x, int y, int type = -1) {
  if (x < BLOCKS_WIDE && y < BLOCKS_HIGH && x >= 0 && y >= 0) {
    if ((type == -1 || MyBlocks[x][y].getType() == type) &&
        !MyBlocks[x][y].getSelected() && MyBlocks[x][y].getType() != 6) {
      // Select it
      MyBlocks[x][y].setSelected (true);

      // Select 4 around it
      int btype = MyBlocks[x][y].getType();
      return 1 + select_block (x - 1, y, btype) + select_block (x + 1, y, btype) + select_block (x, y - 1, btype) + select_block (x, y + 1, btype);
    }
  }

  return 0;
}

// Destroy selected
void game::destroy_selected_blocks() {
  // Remove blocks
  int num_destroyed = 0;

  for (int i = 0; i < BLOCKS_WIDE; i++) {
    for (int t = 0; t < BLOCKS_HIGH; t++) {
      if (MyBlocks[i][t].getSelected()) {
        MyBlocks[i][t].setSelected (false);
        MyBlocks[i][t].explode (particles);
        MyBlocks[i][t].setType (6);
        num_destroyed ++;
      }
    }
  }

  if (config_sound) {
    play_sample (block_break, 64 + num_destroyed * 5, 100, 900 + num_destroyed * 80, 0);
  }

  // Settle blocks downwards
  for (int i = 0; i < BLOCKS_WIDE; i++) {
    int num_blank = BLOCKS_HIGH - 1;

    for (int t = BLOCKS_HIGH - 1; t >= 0; t--)
      if (MyBlocks[i][t].getType() != 6) {
        MyBlocks[i][num_blank--].setType (MyBlocks[i][t].getType());
      }

    while (num_blank >= 0) {
      MyBlocks[i][num_blank--].setType (6);
    }
  }

  // Settle blocks across
  int num_back = 0;

  for (int i = 0; i < BLOCKS_WIDE; i++) {
    for (int t = 0; t < BLOCKS_HIGH; t++) {
      MyBlocks[i - num_back][t].setType (MyBlocks[i][t].getType());
    }

    if (MyBlocks[i][BLOCKS_HIGH - 1].getType() == 6) {
      num_back ++;
    }
  }

  while (num_back > 0) {
    for (int t = 0; t < BLOCKS_HIGH; t++) {
      MyBlocks[BLOCKS_WIDE - num_back][t].setType (6);
    }

    num_back--;
  }
}

// Block at x y
Block *game::block_at (int x, int y) {
  if (x < BLOCKS_WIDE && y < BLOCKS_HIGH && x >= 0 && y >= 0) {
    return &MyBlocks[x][y];
  }

  return nullptr;
}

// Remaining blocks
int game::count_blocks() {
  int blocks_left = 0;

  for (int i = 0; i < BLOCKS_WIDE; i++)
    for (int t = 0; t < BLOCKS_HIGH; t++)
      if (MyBlocks[i][t].getType() != 6) {
        blocks_left ++;
      }

  return blocks_left;
}

// Moves left
int game::count_remaining_moves() {
  int matchesLeft = 0;

  for (int i = 0; i < BLOCKS_WIDE; i++)
    for (int t = 0; t < BLOCKS_HIGH; t++)
      if (MyBlocks[i][t].getType() != 6)
        if ((i < 13 && MyBlocks[i][t].getType() == MyBlocks[i + 1][t].getType()) ||
            (i > 0  && MyBlocks[i][t].getType() == MyBlocks[i - 1][t].getType()) ||
            (t < 8  && MyBlocks[i][t].getType() == MyBlocks[i][t + 1].getType()) ||
            (t > 0  && MyBlocks[i][t].getType() == MyBlocks[i][t - 1].getType())) {
          matchesLeft++;
        }

  return matchesLeft;
}

// Block index
game::coordinate game::get_block_index (int screen_x, int screen_y) {
  for (int i = 0; i < BLOCKS_WIDE; i++) {
    for (int t = 0; t < BLOCKS_HIGH; t++) {
      if (MyBlocks[i][t].getX() < screen_x && MyBlocks[i][t].getY() < screen_y
          && MyBlocks[i][t].getX() + MyBlocks[i][t].getWidth() > screen_x
          && MyBlocks[i][t].getY() + MyBlocks[i][t].getHeight() > screen_y) {
        return coordinate (i, t);
      }
    }
  }

  return coordinate (-1, -1);
}

// Update
void game::update() {
  // Animation for start of game
  if (startAnimate > 10) {
    startAnimate -= 10;
  }
  else {
    startAnimate = 0;

    if (!game_time.IsRunning())
      game_time.Start();
  }

  // In Game
  if (!game_over) {
    // Update particles
    particles.update (16);
    done.Update();

    // Select blocks
    if (MouseListener::mouse_pressed & 1) {
      coordinate selected_block = get_block_index (MouseListener::x, MouseListener::y);

      if (selected_block.x != -1) { // Ensure existing block
        if (blocks_selected > 1 && block_at (selected_block.x, selected_block.y) -> getSelected()) {
          destroy_selected_blocks();
          blocks_selected = 0;
        }
        else {
          deselect_blocks();
          blocks_selected = select_block (selected_block.x, selected_block.y); // Select and count blocks

          if (!config_double_click && blocks_selected > 1 && block_at (selected_block.x, selected_block.y) -> getSelected()) {
            destroy_selected_blocks();
            blocks_selected = 0;
          }
        }
      }

      // Quit
      if (done.Hover()) {
        game_over = true;
        clear_keybuf();
        game_over_message = "Game Over";
      }
    }

    // Lose
    if (count_remaining_moves() == 0) {
      if (count_blocks() == 0) {
        game_over_message = "You Win!";
      }
      else {
        game_over_message = "Out of moves";
      }

      clear_keybuf();
      game_over = true;
    }
  }

  // Game over state
  else {
    if (game_time.IsRunning())
      game_time.Stop();

    // Set score
    if (score == 0) {
      score = difficulty * (((BLOCKS_WIDE * BLOCKS_HIGH) - count_blocks()) - game_time.GetElapsedTime<seconds>());
    }
    else if (score <= 0) {
      score = 1;
    }

    // Name input
    ib_name.Focus();
    ib_name.Update();
    dialog_yes.Update();
    dialog_no.Update();
  }
}


// Draw
void game::draw() {
  // Draws background
  draw_sprite (buffer, background, 0, 0);

  // Draws Blocks
  for (int i = 0; i < 14; i++) {
    for (int t = 0; t < 9; t++) {
      MyBlocks[i][t].draw (buffer, startAnimate);
    }
  }

  // Draw particles
  particles.draw (buffer);

  // Draws foreground
  set_alpha_blender();
  draw_trans_sprite (buffer, foreground, 0, 0);

  // Draw done button
  done.Draw (buffer);

  // Draws text
  textprintf_right_ex (buffer, font, 1240, 16, makecol (0, 0, 0), -1, "Blocks Left: %i", count_blocks());
  textprintf_ex (buffer, font, 40, 16, makecol (0, 0, 0), -1, "Time: %.0f", game_time.GetElapsedTime<seconds>());

  // End game dialog
  if (game_over) {
    // Blur background
    set_alpha_blender();
    draw_trans_sprite (buffer, trans_overlay, 0, 0);

    // Create gui
    draw_sprite (buffer, dialog_box, 300, 300);

    set_alpha_blender();
    draw_trans_sprite (buffer, foreground, 0, 0);

    textprintf_centre_ex (buffer, font, 640, 310, makecol (0, 0, 0), -1, "%s", game_over_message.c_str());
    textprintf_centre_ex (buffer, font, 640, 360, makecol (0, 0, 0), -1, "Score: %i", score);

    // Input rectangle
    ib_name.Draw(buffer);

    // Buttons
    dialog_yes.Draw (buffer);
    dialog_no.Draw (buffer);
  }

  // Draws Cursor
  draw_sprite (buffer, cursor[ (mouse_b & 1)], MouseListener::x, MouseListener::y);

  // Buffer
  stretch_sprite (screen, buffer, 0, 0, SCREEN_W, SCREEN_H);
}
