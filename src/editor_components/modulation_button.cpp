/* Copyright 2013-2015 Matt Tytel
 *
 * helm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * helm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with helm.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "modulation_button.h"
#include "synth_gui_interface.h"

namespace {
  enum MenuIds {
    kCancel = 0,
    kDisconnect,
    kModulationList
  };

} // namespace

ModulationButton::ModulationButton(String name) : ToggleButton(name) {
  setBufferedToImage(true);
}

void ModulationButton::mouseDown(const MouseEvent& e) {
  if (e.mods.isPopupMenu()) {
    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
    std::vector<mopo::ModulationConnection*> connections;
    if (parent == nullptr)
      return;

    connections = parent->getSourceConnections(getName().toStdString());
    if (connections.size() == 0)
      return;

    PopupMenu m;
    String disconnect("Disconnect from ");
    for (int i = 0; i < connections.size(); ++i)
      m.addItem(kModulationList + i, disconnect + connections[i]->destination);

    if (connections.size() > 1)
      m.addItem(kDisconnect, "Disconnect all");

    int result = m.show();
    if (result == kDisconnect) {
      for (mopo::ModulationConnection* connection : connections)
        parent->disconnectModulation(connection);
      repaint();
    }
    else if (result >= kModulationList) {
      int connection_index = result - kModulationList;
      parent->disconnectModulation(connections[connection_index]);
      repaint();
    }
  }
  else
    ToggleButton::mouseDown(e);
}

void ModulationButton::mouseUp(const MouseEvent& e) {
  if (!e.mods.isPopupMenu())
    ToggleButton::mouseUp(e);
}