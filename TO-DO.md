TO-DO List

1. Ability to select several objects.
   This may or may not include the ability to drag or move them.
   (DO NOT IMPLEMENT)
2. Use QApplication::processEvents() to generate the source code.
3. Do not allow user to drag from one window into another. This error
   creates a segmentation fault. (DONE) Solution: do not to delete
   the drag object.
4. Warn the user if they are about to exit an unsaved session. (DONE)
5. Open & Save functionality of QtNQC diagram code. Also design and
   implement a file format. (DONE)
6. Right-clicking on the diagram should allow the user to view a listing
   of all items on that canvas. The panel should list the objects
   by name as well as their location. A "goto object" option should
   also be provided. Delete option also included. (DONE)
7. Implement a Help section.
8. Display coordinates when an object is selected. (DONE)
9. Implement an undo feature.
10. Functions and tasks can only be defined on the main canvas. (DONE)
11. Allow user to rename functions and tasks, assign default names
    initially. (DONE)
12. Implement the NQC API.
13. Display a list of available functions and tasks. This will allow the
    user to select these functions when working outside the main canvas.
    Create a sub menu. (DONE)
14. Allow user to add defined tasks and/or functions to canvases.
15. Prepend an "f_" or a "t_" for functions and tasks during NQC
    generation.
16. Allow user to rename variables. If the user renames a variable
    to a global variable's name, use the global variable's name. (DONE)
17. Add a true/false case structure.
18. Add an option to raise the main window. (DONE)
19. Fix dragging & dropping after a scroll event. (DONE)
20. Implement a wiring mechanism.
21. Save progress to a temporary file, if possible, when exiting
    abnormally. Thus, set signal handlers for dangerous signals.
    This can already be handled by the auto-save mechanism.
    (DO NOT IMPLEMENT)
22. Keep wires and objects together after a move.
23. Prevent move/drag/add collisions. (DONE)
24. Implement a "new" function. This should clear all containers. (DONE)
25. Only allow user to wire an output to an input.
26. Do not allow user to wire more than one object to an input.
27. Implement the Delete cursor. (DONE)
28. Scroll when dragging.
29. Use a Constant's value as its name. (DONE)
30. Verify input value for Constants. (DONE)
31. Allow user to define Variables as globals. This should only be allowed
    in the main Editor. Global variables should be clearly marked. (DONE)
32. Force all memory allocation failures to exit the program. (DONE)
33. Set an atexit() function to handle abnormal exits.
34. Double-clicking on an object should call its update method. (DONE)
35. Allow user to save settings. Application should create a .qtnqc.conf
    file in their $HOME. The contents of the file should be text-based.
    Use the QSettings class.
36. Implement the AI required to build the NQC code from the diagram.
37. Correct inputs for the various loop structures. These should not be
    wired in values, but something the user needs to type in. These
    inputs will be the "conditions." (DONE)
38. Add shortcut keys to file menus. (DONE)
39. Provide the ability to resize the canvas of other objects. Currently,
    only the main canvas may be resized. (DONE)
40. Implement a search utility. The search should run in the background, that
    is, use a separate thread.
41. Create a splash panel. Not really necessary. (DO NOT IMPLEMENT)
42. Allow user to display a grid on the canvas. (DO NOT IMPLEMENT)
43. Make the rename_window class part of the editor class. (DONE)
44. Variables defined on the main canvas will be global. Any variable
    having the same name as a variable on the main canvas will not be
    a local variable. If the user deletes or renames global variables,
    non-local variables having those names will become local variables.
45. Look into fixing the color problems in the Tools & Functions
    dialogs. (DONE)
46. Provide support for arrays.
47. Create the if and switch statement objects.
48. Create a thread which writes the contents of the current diagram
    to a temporary file in /tmp. The write interval should be user-defined.
49. When adding a function or task from the "existing" submenu, add the
    object correctly, that is, this function must be implemented.
50. Set a callback to the Window Close option. Use the quit(void) method.
    (DONE)
51. Add a "Software Version" option. Use the libcurl library. (DONE)
52. Save the size of the canvas to the data file. (DONE)
53. Allow the user to copy & paste objects.
54. Create an option that allows the user to upload the NQC program
    directly to the RCX brick. It should be able to display a progress
    bar. The location of the NQC executable should be configurable.
55. Implement the break object. (DONE)
56. Implement the subroutine object. (DONE)
57. Implement the continue object. (DONE)
58. Implement the return object. (DONE)
59. Implement access controls and events.
60. Implement the start and stop objects.
61. Right-clicking on the canvas should not set the program in an
    unsaved state.
62. Renaming functions, subroutines, and tasks should update the Canvas Items
    panel. (DONE)
63. Reset the main canvas to its original size when creating a new
    diagram. (DONE)
64. Implement "arguments" support for functions and tasks.
65. Closing a loop's window should not close the window(s) of any
    children of that loop. This appears to be Qt's natural
    behaviour. (IGNORE)
66. Items that should be configurable:
    a) Save contents every X number of seconds or minutes.
    b) Location of the main window. (DONE)
67. Prompt the user for delete confirmation before deleting an object
    from the canvas. Add an "editing" tab to the program settings
    panel to disable to prompt. (DONE)
68. Provide support for all of the robots that the NQC language supports.
    QtNQC will have to disable/enable certain functionality based on the
    robot type.
69. Create a "project" panel. This panel is to be used for selecting
    the type of robot the project is intended for.
70. Implement the paste functionality for loops.
71. editor::cleanup() should delete the copied_item object and reset
    copied_item_type and selected_item_type. (DONE)
72. Implement the paste functionality for variables.
73. Investigate the cause of "warning: junk pointer, too high to make sense"
    after checking the version of the program. (DONE)
    Incorrect use of free in qtnqc_download_thread::run().
