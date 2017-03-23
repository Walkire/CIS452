
import sys, time
import curses

height = 0
width = 0
maxMole = 0
upDura = 0
downDura = 0
stdscr = curses.initscr()

def main():
	#Checks arg amount
	if len(sys.argv) < 6:
		print("Incorrect number of arguments, Usage: "+sys.argv[0]+" [height] [width] [max #of moles] "+
                        "[mole pop-up duration(milisec)] [mole hide duration(milisec)]")
		endCurses()
		sys.exit()

	#Sets global
	height = sys.argv[1]
	width = sys.argv[2]
	maxMole = sys.argv[3]
	upDura = sys.argv[4]
	downDura = sys.argv[5]

	createCurses()
	stdscr.refresh()
	time.sleep(1)
	endCurses()

def createCurses():
	curses.noecho()	
	stdscr.clear()
	stdscr.refresh()

def endCurses():
	curses.nocbreak()
	stdscr.keypad(False)
	curses.echo()
	curses.endwin()

if __name__ == "__main__":
	main()	
