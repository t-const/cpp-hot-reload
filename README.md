# C++ Hot Reload

Library used to reload another dynamic library while application using it is still running.

A potential use case could be:

Change OpenCV image manipulation c++ code in realtime.

## Todo

- [ ] Add the unit test stuff from the tuto
- [ ] Look on how to ease the process to maintin the public interface
  - [ ] Manually specify methods metadata
  - [ ] Manually specify function index
  - [ ] Manually specify execute vs var
- [ ] Add mechanisms of multi threading when lib is reloaded and lib functions are in uses
- [ ] Add file watcher to reload lib when file is saved and is in lib (from vscode ?)

## Next steps

- [ ] Define necessary cmake files to include this to other projects
- [ ] Add to another project
  - [ ] Make sure project is minimal and easy to share/include
- [ ] Support different config (windows)
