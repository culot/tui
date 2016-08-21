/*-
 * Copyright (c) 2016 Frederic Culot <culot@FreeBSD.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer
 *    in this position and unchanged.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR(S) ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR(S) BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <iostream>
#include <curses.h>

#include "window.h"


int main(int argc, char** argv) {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  refresh();

  tui::Window myWin(COLS, 10, 0, 0);
  for (auto i = 0; i < 10; ++i) {
    myWin.print("Lorem ipsum dolor sit amet,");
    myWin.print("consectetur adipiscing elit,");
    myWin.print("sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
    myWin.print("Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.");
    myWin.print("Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.");
    myWin.print("Excepteur sint occaecat cupidatat non proident,");
    myWin.print("sunt in culpa qui officia deserunt mollit anim id est laborum.");
  }
  myWin.draw();

  tui::Window myWin2(COLS, LINES - 11, 0, 10);
  myWin2.draw();

  refresh();

  for (;;) {
    switch (getch()) {
    case KEY_DOWN:
      myWin.moveCursorDown();
      break;
    case KEY_UP:
      myWin.moveCursorUp();
      break;
    case 'q':
      clear();
      endwin();
      return 0;
    }
    myWin.draw();
    myWin2.draw();
  }
}
