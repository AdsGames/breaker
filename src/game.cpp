#include "game.h"

// INIT
game::game() {
  // Sets Cursors
  cursor[0] = load_png ("images/cursor1.png", NULL);
  cursor[1] = load_png ("images/cursor2.png", NULL);

  // Creates a buffer
  buffer = create_bitmap (1280, 960);

  // Sets background
  background = load_png ("images/background.png", NULL);

  // Sets Foreground
  foreground = load_png ("images/foreground.png", NULL);

  // Sets menu
  dialog_box = load_png ("images/menu.png", NULL);

  // Trans overlay
  trans_overlay = load_png ("images/overlay.png", NULL);

  // Sets Sounds
  block_break = load_sample ("sounds/break.wav");
  click = load_sample ("sounds/click.wav");

  // Sets Variables
  score = 0;
  startAnimate = 1200;
  blocks_selected = 0;
  game_over = false;
  game_over_message = "Game Over";
  edittext = "Player";
  iter = edittext.end();

  // Resets Timers
  startTime = clock();
  currentTime = clock();
  lastTickTime = clock();

  // Sets block info
  for (int i = 0; i < 14; i++) {
    for (int t = 0; t < 9; t++) {
      MyBlocks[i][t].setType (random (0, difficulty));
      MyBlocks[i][t].setSelected (false);
      MyBlocks[i][t].setX (i * 80 + 80);
      MyBlocks[i][t].setY (t * 80 + 80);
    }
  }

  // Sets block images
  Block::images[0] = load_png ("images/blocks/red.png", NULL);
  Block::images[1] = load_png ("images/blocks/orange.png", NULL);
  Block::images[2] = load_png ("images/blocks/yellow.png", NULL);
  Block::images[3] = load_png ("images/blocks/green.png", NULL);
  Block::images[4] = load_png ("images/blocks/blue.png", NULL);
  Block::images[5] = load_png ("images/blocks/purple.png", NULL);
  Block::images[6] = load_png ("images/blocks/none.png", NULL);
  Block::images[7] = load_png ("images/blocks/flash.png", NULL);

  // Give score files
  highscores = scoreTable ("scores.dat");
  highscores.load();

  // Buttons
  done.SetImages ("images/buttons/done.png", "images/buttons/done_hover.png");
  dialog_yes.SetImages ("images/buttons/yes.png", "images/buttons/yes_hover.png");
  dialog_no.SetImages ("images/buttons/no.png", "images/buttons/no_hover.png");

  done.SetPosition (500, 10);
  dialog_yes.SetPosition (41 + 300, 215 + 300);
  dialog_no.SetPosition (461 + 300, 215 + 300);
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

  for (int i = 0; i < 8; i++) {
    destroy_bitmap (Block::images[i]);
  }
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
  for (int i = 0; i < BLOCKS_WIDE; i++) {
    for (int t = 0; t < BLOCKS_HIGH; t++) {
      if (MyBlocks[i][t].getSelected()) {
        MyBlocks[i][t].setSelected (false);
        MyBlocks[i][t].explode (particles);
        MyBlocks[i][t].setType (6);

        if (config_sound) {
          play_sample (block_break, 64, 100, random (400, 2000), 0);
        }
      }
    }
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

  return NULL;
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
  }

  // In Game
  if (!game_over) {
    // Updates Elasped Time
    currentTime = clock();
    elaspedTime = int (currentTime - startTime) / CLOCKS_PER_SEC;

    // Update particles
    particles.update (currentTime - lastTickTime);

    lastTickTime = currentTime;

    // Select blocks
    if (mouseListener::mouse_pressed & 1) {
      coordinate selected_block = get_block_index (mouseListener::res_mouse_x, mouseListener::res_mouse_y);

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

      game_over = true;
    }
  }

  // Game over state
  else {
    // Set score
    if (score == 0) {
      score = difficulty * (((BLOCKS_WIDE * BLOCKS_HIGH) - count_blocks()) - elaspedTime);
    }

    if (score <= 0) {
      score = 1;
    }

    // Checks for button press
    if (mouseListener::mouse_pressed & 1) {
      if (dialog_yes.Hover()) {
        highscores.addScore (edittext, score);
        set_next_state (STATE_GAME);
      }
      else if (dialog_no.Hover()) {
        highscores.addScore (edittext, score);
        set_next_state (STATE_MENU);
      }
    }

    // Name input
    if (keypressed()) {
      int  newkey   = readkey();
      char ASCII    = newkey & 0xff;
      char scancode = newkey >> 8;

      // a character key was pressed; add it to the string
      if (ASCII >= 32 && ASCII <= 126 && edittext.length() < 14 && scancode != KEY_SPACE) {
        // add the new char
        iter = edittext.insert (iter, ASCII);
        // increment both the caret and the iterator
        iter++;
      }
      // some other, "special" key was pressed; handle it here
      else {
        if (scancode == KEY_DEL)
          if (iter != edittext.end()) {
            iter = edittext.erase (iter);
          }

        if (scancode == KEY_BACKSPACE) {
          if (iter != edittext.begin()) {
            iter--;
            iter = edittext.erase (iter);
          }
        }

        if (scancode == KEY_RIGHT)
          if (iter != edittext.end()) {
            iter++;
          }

        if (scancode == KEY_LEFT)
          if (iter != edittext.begin()) {
            iter--;
          }

        if (scancode == KEY_ENTER) {
          highscores.addScore (edittext, score);
          set_next_state (STATE_MENU);
        }
      }
    }
  }
}


// Draw
void game::draw() {
  // Draws background
  draw_sprite (buffer, background, 0, 0);

  // Draws Blocks
  for (int i = 0; i < 14; i++)
    for (int t = 0; t < 9; t++) {
      MyBlocks[i][t].draw (buffer, startAnimate);
    }

  // Draws foreground
  set_alpha_blender();
  draw_trans_sprite (buffer, foreground, 0, 0);

  // Draw done button
  done.draw (buffer);

  // Draw particles
  particles.draw (buffer);

  // Draws text
  textprintf_right_ex (buffer, font, 1240, 16, makecol (0, 0, 0), -1, "Blocks Left: %i", count_blocks());
  textprintf_ex (buffer, font, 40, 16, makecol (0, 0, 0), -1, "Time: %i", elaspedTime);

  // End game dialog
  if (game_over) {
    // Clear particle emitter particles
    particles.clear_all();

    // Blur background
    set_alpha_blender();
    draw_trans_sprite (buffer, trans_overlay, 0, 0);

    // Create gui
    draw_sprite (buffer, dialog_box, 300, 300);

    set_alpha_blender();
    draw_trans_sprite (buffer, foreground, 0, 0);

    textprintf_centre_ex (buffer, font, 640, 310, makecol (0, 0, 0), -1, game_over_message.c_str());
    textprintf_centre_ex (buffer, font, 640, 360, makecol (0, 0, 0), -1, "Score: %i", score);

    // Input rectangle
    rectfill (buffer, 488, 408, 892, 452, makecol (0, 0, 0));
    rectfill (buffer, 490, 410, 890, 450, makecol (255, 255, 255));

    // output the string to the screen
    textout_ex (buffer, font, edittext.c_str(), 494, 410, makecol (0, 0, 0), -1);

    // draw the caret
    vline (buffer, text_length (font, edittext.c_str()) + 494, 412, 448, makecol (0, 0, 0));

    // Buttons
    dialog_yes.draw (buffer);
    dialog_no.draw (buffer);
  }

  // Draws Cursor
  draw_sprite (buffer, cursor[ (mouse_b & 1)], mouseListener::res_mouse_x, mouseListener::res_mouse_y);

  // Buffer
  stretch_sprite (screen, buffer, 0, 0, SCREEN_W, SCREEN_H);
}
