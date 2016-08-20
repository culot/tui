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
  WINDOW*  win    {nullptr};
  int      height {LINES};
  int      width  {COLS};

  int      ypos   {0};
  int      xpos   {0};
  bool     border {true};
};


Window::Window()
  : impl_{new Impl} {
  impl_->win = newwin(impl_->height, impl_->width, impl_->ypos, impl_->xpos);
}

Window::Window(int width, int height, int xpos, int ypos)
  : Window() {
  resize(width, height);
  move(xpos, ypos);
}

Window::~Window() {
  delwin(impl_->win);
}

void Window::draw() {
  if (impl_->border) {
    box(impl_->win, 0, 0);
  }
  wrefresh(impl_->win);
}

void Window::resize(int width, int height) {
  impl_->width = width;
  impl_->height = height;
  wresize(impl_->win, impl_->height, impl_->width);
}

void Window::move(int xpos, int ypos) {
  impl_->xpos = xpos;
  impl_->ypos = ypos;
  mvwin(impl_->win, impl_->ypos, impl_->xpos);
}

void Window::toggleBorder() {
  impl_->border = impl_->border ? false : true;
}

}
