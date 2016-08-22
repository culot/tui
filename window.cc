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

#include <curses.h>

#include "window.h"


namespace tui {

class Window::Impl {
 public:
  // Need to use both an curses win structure (to be able to draw the border),
  // and a curses pad structure (to display the window contents.
  // If only using a pad, and applying the border directly to it, the bottom
  // line of the border gets overwritten with the pad content.
  WINDOW* win {nullptr};
  WINDOW* pad {nullptr};

  /*
     +------------------------------------------+-  -  -  -v
     |                                          |          | yView
     |      +-  +-----------------+ -  -  -  -  |-v -  -  -| -  -  v
     |      |   |abc              |             | | y      |       |
     |      |   +-----------------+ --+ -  -  - |-^ -  -v -^       |
     |    h |   |def              |   |         |       |          |
     |    e |   |ghi              |   |height   |       | yCursor  |
     |    i |   |jkl              |   |View     |       |          |
     |    g |   |mno##############| - | -  -  - |-  -  -^          | yPrint
     |    h |   +-----------------+  -+         |                  |
     |    t |   |pqr              |             |                  |
     |      |   |stu              |             |                  |
     |      |   |                 | -  -  -  -  |-  -  -  -  -  -  ^
     |      +-  +-----------------+             |
     |                                          |
     |                                   screen |
     +------------------------------------------+
  */
  int height {LINES};
  int width {COLS};
  int heightView {LINES};
  int widthView {COLS};

  int y {0};
  int x {0};
  int yView {0};
  int xView {0};
  int yCursor {0};
  int yPrint {0};

  bool cursorLineHighlighted {true};

  void highlightCursorLine();
  void unhighlightCursorLine();
};


Window::Window()
  : impl_{new Impl} {
  impl_->win = newwin(impl_->height, impl_->width, impl_->yView, impl_->xView);
  impl_->pad = newpad(impl_->height - 2, impl_->width - 2); // -2 for borders
}

Window::Window(int width, int height, int x, int y)
  : Window() {
  resize(width, height);
  move(x, y);
}

Window::~Window() {
  delwin(impl_->pad);
  delwin(impl_->win);
}

void Window::cursorLineHighlighted(bool highlight) {
  impl_->cursorLineHighlighted = highlight;
}

void Window::draw() {
  box(impl_->win, 0, 0);
  wnoutrefresh(impl_->win);
  impl_->highlightCursorLine();
  pnoutrefresh(impl_->pad,
               impl_->y,
               impl_->x,
               impl_->yView + 1,
               impl_->xView + 1,
               impl_->yView + impl_->heightView - 2,
               impl_->xView + impl_->widthView - 2);
  refresh();
}

void Window::print(const std::string& line) {
  mvwaddstr(impl_->pad, impl_->yPrint++, impl_->x, line.c_str());
}

void Window::resize(int width, int height) {
  impl_->widthView = width;
  impl_->heightView = height;
  wresize(impl_->win, impl_->heightView, impl_->widthView);
}

void Window::move(int x, int y) {
  impl_->xView = x;
  impl_->yView = y;
  wmove(impl_->win, impl_->yView, impl_->xView);
}

void Window::scrollDown() {
  if (impl_->yCursor >= impl_->heightView - 2
      && impl_->yPrint > impl_->y + impl_->heightView) {
    ++impl_->y;
  }
}

void Window::scrollUp() {
  if (impl_->yCursor < impl_->y && impl_->y > 0) {
    --impl_->y;
  }
}

  void Window::moveCursorDown() {
  if (impl_->yCursor + impl_->y < impl_->height
      && impl_->y + impl_->yCursor - 1 < impl_->yPrint) {
    impl_->unhighlightCursorLine();
    ++impl_->yCursor;
  }
  scrollDown();
}

void Window::moveCursorUp() {
  if (impl_->yCursor > 0) {
    impl_->unhighlightCursorLine();
    --impl_->yCursor;
  }
  scrollUp();
}

void Window::Impl::highlightCursorLine() {
  if (cursorLineHighlighted) {
    mvwchgat(pad, yCursor, 0, width, A_REVERSE, 0, nullptr);
  }
}

void Window::Impl::unhighlightCursorLine() {
  if (cursorLineHighlighted) {
    mvwchgat(pad, yCursor, 0, width, A_NORMAL, 0, nullptr);
  }
}

}
