doxygen -w html new_header.html new_footer.html new_stylesheet.css
doxygen.exe doxyconfig.cfg

rmdir /q /s Docs\html
rmdir /q /s Docs\latex
move html Docs
move latex Docs