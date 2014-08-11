//  SuperTux
//  Copyright (C) 2013 Ingo Ruhnke <grumbel@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "supertux/game_manager.hpp"

#include <sstream>

#include "gui/menu_manager.hpp"
#include "lisp/lisp.hpp"
#include "lisp/parser.hpp"
#include "supertux/game_session.hpp"
#include "supertux/gameconfig.hpp"
#include "supertux/globals.hpp"
#include "supertux/screen.hpp"
#include "supertux/screen_fade.hpp"
#include "supertux/screen_manager.hpp"
#include "supertux/world.hpp"
#include "util/file_system.hpp"
#include "util/log.hpp"

GameManager::GameManager() :
  m_world()
{
}

GameManager::~GameManager()
{
}

void
GameManager::start_level(std::unique_ptr<World> world, int index)
{
  m_world = std::move(world);

  std::unique_ptr<Screen> screen(new GameSession(m_world->get_level_filename(index),
                                                 m_world->get_player_status()));
  g_screen_manager->push_screen(std::move(screen));
}

void
GameManager::start_game(std::unique_ptr<World> world)
{
  m_world = std::move(world);

  MenuManager::instance().clear_menu_stack();

  try
  {
    m_world->run();
  }
  catch(std::exception& e)
  {
    log_fatal << "Couldn't start world: " << e.what() << std::endl;
  }
}

std::string
GameManager::get_level_name(const std::string& filename) const
{
  try
  {
    lisp::Parser parser;
    const lisp::Lisp* root = parser.parse(filename);

    const lisp::Lisp* level = root->get_lisp("supertux-level");
    if(!level)
      return "";

    std::string name;
    level->get("name", name);
    return name;
  }
  catch(const std::exception& e)
  {
    log_warning << "Problem getting name of '" << filename << "': "
                << e.what() << std::endl;
    return "";
  }
}

/* EOF */
