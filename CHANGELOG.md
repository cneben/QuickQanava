CHANGELOG
---------

**20180529:**

- Fix all tests in GTpo, increase coverage for groups.
- Reactivate qan::Graph::removeGroup() method (see groups sample).
- Minor fixes in topology and dataflow samples.

**20180527:**

- Add the cpp sample to demonstrate the C++ API, add a complete custom group implementation sample.
- BUGFIX: Node fixed resizing ratio modification was not taken into account for currently selected node.
- BUGFIX: Bottom right resizer is now hidden when current node is unselected.
- BUGFIX: Bottom right resizer item for nodes and group is now shown on top of selection item.
- BUGFIX: When the currently selected group selection policy is set to unselectable, unselect the group.
- Add minimumSize and resizable properties to Qan.GroupItem (see groups sample).
- Expose resizeHandlerOpacity, resizeHandlerRadius, resizeHandlerWidth and resizeHandlerSize properties from Qan.GraphView to allow
  complete customization of bottom right resizer handler (works for both nodes and groups).
- Add support for Group.labelEditorVisible and Group.expandButtonVisible in Qan.Group and Qan.RectGroupTemplate.
- Default color for selection items, bottom right resizer and visual node connector is now dodgerblue (previously darkblue).
- Add qan::NodeItem::setRect(const QRectF&) and qan::GroupItem(const QRectF&) shortcut methods.
- Various cosmetics fixes in qan::Graph::removeFromSelection() and qan::Graph::removeFromSelectionImpl<>().
- Major cleaning of the groups sample and demonstration of new group styling and resizing options.

**20180429:**

- Major documentation update (see Graph Data Model and Node Style.

**20180326:** 

- Add style support for gradient fill and glow effect. Reactivate style management (see style sample).
