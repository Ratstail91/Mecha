REM Use the output tool to convert the .csv file into image files

REM TODO

REM Iterate over each file in img\, passing it to convert to change the PPI to 150
REM Convert can be found at www.imagemagick.org

for /f "delims=|" %%f in ('dir /B img') do convert -density 150 -units pixelsperinch "img\%%f" "img\%%f"