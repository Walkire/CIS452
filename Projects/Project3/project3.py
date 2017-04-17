#!/user/bin/env python3
import sys, time, threading, random, signal
import curses
from curses import wrapper

MOLEONBOARD={}
THREAD_LIST=[]
RAND_LINE=[]
RAND_COL=[]
SIGINT = 1
GAMESTART = 1

def main(stdscr):
	global MAX_MOLE
	global semaphore
	global BOARD
	
	#assigns the curses to BOARD
	BOARD = stdscr
	#Creates sig handler
	signal.signal(signal.SIGINT, signal_handler)
	#Creates the semaphore
	semaphore = threading.BoundedSemaphore(MAX_MOLE)
	createBoard()
	createThreads()
	gameEngine()

def gameEngine():
	global LINE
	global RAND_LINE
	global RAND_COL
	global GAMESTART
	global MOLEONBOARD
	global BOARD

	molesHit = 0

	#Game will begin if user hits s
	while GAMESTART:
		read = BOARD.getch()
		if read == ord('s'):
			BOARD.addstr(5+(LINE*4),0,"Moles hit:              ")
			BOARD.addstr(5+(LINE*4),12,str(molesHit))
			GAMESTART = 0

	#Constantly reads the characters read by the user
	while True:
		read = BOARD.getch()
		if chr(read) in MOLEONBOARD:
			foundChr = chr(read)
			molesHit=molesHit+1
			BOARD.addstr(5+(LINE*4),12,str(molesHit))
			BOARD.addch(2+(RAND_LINE[MOLEONBOARD[foundChr]]*4),2+(RAND_COL[MOLEONBOARD[foundChr]]*4),' ')
			resetPosition()
			try:
				del MOLEONBOARD[chr(read)]
			except:
				pass

#Constantly refreshes the board
def boardRefresh():
	while GAMESTART:
		time.sleep(0.1)
	while SIGINT:
		BOARD.refresh()
		time.sleep(0.01)

def waitingRoom(moleChar, threadNum):
	global UP_DURA
	global SIGINT

	#Before the game starts the threads sit here
	while(GAMESTART):
		time.sleep(.1)

	while(SIGINT):
		time.sleep(random.uniform(0,DOWN_DURA/1000))
		popUp(moleChar, threadNum)

	print("Thread ["+str(threadNum+1)+"] ending")

def popUp(moleChar, threadNum):
	global semaphore
	global UP_DURA
	global MOLEONBOARD
	global BOARD
	global LINE
	global COL
	global RAND_LINE
	global RAND_COL

	#Semaphore lock
	semaphore.acquire()
	#Gives a random x and y position for each thread
	RAND_LINE[threadNum] = random.randrange(0,LINE)
	RAND_COL[threadNum] = random.randrange(0,COL)
	#Adds the character to the board
	BOARD.addch(2+(RAND_LINE[threadNum]*4),2+(RAND_COL[threadNum]*4),moleChar)
	resetPosition()
	MOLEONBOARD[moleChar] = threadNum
	time.sleep(random.uniform(0,UP_DURA/1000))
	try:
		del MOLEONBOARD[moleChar]
	except:	
		pass
	BOARD.addch(2+(RAND_LINE[threadNum]*4),2+(RAND_COL[threadNum]*4),' ')
	resetPosition()
	#semaphore release
	semaphore.release()

def createThreads():
	global THREADS
	
	#List of mole characters
	moleChar = [
		"a","b","c","d","e",
		"f","g","h","i","j",
		"k","l","m","n","o",
		"p","q","r","s","t",
		"u","v","w","x","y",
		"z","0","1","2","3",
		"4","5","6","7","8","9"]
	#Creates the threads and sends them to waitingRoom puts all the threads on a list
	try:
		for x in range(THREADS):
			t = threading.Thread(target=waitingRoom, args=(moleChar[x],x,))
			THREAD_LIST.append(t)
			t.start()
	except:
		raise Exception("Error: unable to start threads")
	#This is an extra thread to be used for updating the game board
	t = threading.Thread(target=boardRefresh, args=())
	t.start()

#Creates the board
def createBoard():
	global BOARD
	global RAND_LINE
	global RAND_COL
	global THREADS
	global LINE
	global COL	
	
	BOARD.clear()
	#Creates the framework of the board
	for i in range((LINE*4)+1):
		for j in range((COL*4)+1):
			if (i%4) == 0:
				if(j%4) == 0:
					BOARD.addch(i,j,'+')
				else:
					BOARD.addch(i,j,'-')
			else:
				if(j%4) == 0:
					BOARD.addch(i,j,'|')
				else:
					BOARD.addch(i,j,' ')

	#Makes sure the size of the two lists are the same as 
	#The amount of threads					
	for i in range(THREADS):
		RAND_LINE.append(i)
		RAND_COL.append(i)
	BOARD.addstr(3+(LINE*4),0,"Hit the keys [a-z] or [0-1] to hit the moles! (Press Ctrl-C to quit)")
	BOARD.addstr(5+(LINE*4),0,"Press the s key to start")
	resetPosition()
	BOARD.refresh()

#puts the cursor back in a default location
def resetPosition():
	global BOARD
	global LINE
	
	BOARD.addch(6+(LINE*4),0,' ')

#Ends the game and lets the threads run through to the end of the waiting room function
def signal_handler(signal, frame):
	global SIGINT
	SIGINT = 0
	GAMESTART = 0
	sys.exit()

#Startup Code
if __name__ == "__main__":
	global LINE
	global COL
	global THREADS
	global MAX_MOLE
	global UP_DURA
	global DOWN_DURA

	#Checks arg amount
	if len(sys.argv) < 7:
		print("Incorrect number of arguments")
		print("Usage: "+sys.argv[0]+" [height] [width] [number of moles] [max #of moles] "+
			"[mole pop-up duration(milisec)] [mole hide duration(milisec)]")
		sys.exit()

	#Sets global and checks for special conditions
	LINE = int(sys.argv[1])
	COL = int(sys.argv[2])
	THREADS = int(sys.argv[3])
	MAX_MOLE = int(sys.argv[4])
	UP_DURA = int(sys.argv[5])
	DOWN_DURA = int(sys.argv[6])

	#Checks to make sure max board size <= 36
	if (LINE*COL) < 4 or (LINE*COL) > 36:
		print("Boardsize is incorrect, boardsize(height*width) must be: >= 4 and <= 36")
		sys.exit()	
	
	#Checks to make sure max number of moles doesnt exceed the max boardsize
	if MAX_MOLE > (LINE*COL):
		print("Max number of moles is larger then the boardsize")
		sys.exit()

	#Chceks to make sure amount of threads doesnt exceed 36
	if THREADS > 36:
		print("Number of moles is too large (Threads < 36)")
		sys.exit()

	wrapper(main)	
