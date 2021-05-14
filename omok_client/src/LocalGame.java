import java.awt.*;
import java.awt.event.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.*;
import javax.swing.border.EmptyBorder;

import Board.*;

public class LocalGame extends JPanel {
	
	private RenjuBoard	boardData		= new RenjuBoard();
	private byte 		turn			= RenjuBoard.BLACK;
	private boolean		clickable		= true;
	private boolean		clicked			= false;	// to notify grid either draw clicked or not
	private Grid	 	clickedGrid		= null;		// point to clicked grid 
	private boolean 	clickLegal		= false;	// test result of clicked grid(x, y)
	private final int 	timeLimitSec;				// given timer limit second value
	private int 		timeTick;					// changed every second, to notify timeLabel
	
	private JPanel 		board			= new JPanel();			// contains grid
	private Grid[][] 	grids			= new Grid[15][15];		// GridListener
	private TimeLabel	timeLabel;								// changed every second
	private TurnLabel	turnLabel;								// changed on turn change
	private JButton 	placeButton		= new JButton("Place!");// PlaceButtonListener
	private Timer 		timeTicker; 
	private Timer		timeResetter;
	private Font		myfont			= new Font("Monospaced", Font.PLAIN, 20);
	
	private BufferedImage empty;
	private BufferedImage emptyN;
	private BufferedImage emptyNE;
	private BufferedImage emptyE;
	private BufferedImage emptySE;
	private BufferedImage emptyS;
	private BufferedImage emptySW;
	private BufferedImage emptyW;
	private BufferedImage emptyNW;
	private BufferedImage black;
	private BufferedImage white;
	private BufferedImage legal; // for click
	private BufferedImage illeg;
	private BufferedImage plate; // for turnLabel
	
	public LocalGame(int timeLimitSec) {
		this.timeLimitSec = timeLimitSec;
		timeTick = timeLimitSec;
		try {
			empty = ImageIO.read(new File("images/empty.png"));
			emptyN = ImageIO.read(new File("images/emptyN.png"));
			emptyNE = ImageIO.read(new File("images/emptyNE.png"));
			emptyE = ImageIO.read(new File("images/emptyE.png"));
			emptySE = ImageIO.read(new File("images/emptySE.png"));
			emptyS = ImageIO.read(new File("images/emptyS.png"));
			emptySW = ImageIO.read(new File("images/emptySW.png"));
			emptyW = ImageIO.read(new File("images/emptyW.png"));
			emptyNW = ImageIO.read(new File("images/emptyNW.png"));			
			black = ImageIO.read(new File("images/black.png"));
			white = ImageIO.read(new File("images/white.png"));
			legal = ImageIO.read(new File("images/legal.png"));
			illeg = ImageIO.read(new File("images/illeg.png"));
			plate = ImageIO.read(new File("images/plate.png"));
		} catch (IOException e) {
			System.out.println(e.getStackTrace());
			empty = null;
			emptyN = null;
			emptyNE = null;
			emptyE = null;
			emptySE = null;
			emptyS = null;
			emptySW = null;
			emptyW = null;
			emptyNW = null;
			black = null;
			white = null;
			legal = null;
			illeg = null;
			plate = null;
		}
		initboard();
		initgrids();
		initelse();
		setLayout(new BorderLayout());
		JPanel box = new JPanel();
		box.setLayout(new BoxLayout(box, BoxLayout.LINE_AXIS));
		box.add(board);
		add(box, BorderLayout.CENTER);
		
		JPanel east = new JPanel();
		east.setLayout(new GridLayout(3, 1));
		east.add(timeLabel);
		east.add(turnLabel);
		east.add(placeButton);
		add(east, BorderLayout.EAST);
	}
	
	private void initboard() {
		board.setLayout(new GridLayout(15, 15, 0, 0));
		board.setBorder(new EmptyBorder(0, 0, 0, 0));
		board.setMaximumSize(new Dimension(750, 750));
	}
	private void initgrids() {
		int i, j;
		// grids[0]
		grids[0][0] = new Grid(1, 1, emptyNW);
		for (j = 1; j < 14; j++)
			grids[0][j] = new Grid(1, j+1, emptyN);
		grids[0][14] = new Grid(1, 15, emptyNE);
		// grids[1-13]
		for (i = 1; i < 14; i++) {
			grids[i][0] = new Grid(i+1, j+1, emptyW);
			for (j = 1; j < 14; j++)
				grids[i][j] = new Grid(i+1, j+1, empty);
			grids[i][j] = new Grid(i+1, j+1, emptyE);
		}
		// grids[14]
		grids[14][0] = new Grid(15, 1, emptySW);
		for (j = 1; j < 14; j++)
			grids[14][j] = new Grid(15, j+1, emptyS);
		grids[14][14] = new Grid(15, 15, emptySE);
		
		// add listener & add to board
		GridListener gl = new GridListener();
		for (i = 0; i < 15; i++)
			for (j = 0; j < 15; j++) {
				grids[i][j].addMouseListener(gl);
				board.add(grids[i][j]);
			}
		
	}
	private void initelse() {
		timeLabel = new TimeLabel(new Color(4, 5, 46), new Color(191, 219, 247));
		timeLabel.setAndRepaint(0, timeTick);
		
		turnLabel = new TurnLabel(black, white, plate);
		
		placeButton.addActionListener(new PlaceButtonListener());
		placeButton.setFont(myfont);
		
		if (timeLimitSec != 0) {
			timeTicker = new Timer(1000, new TimeTickerListener());
			timeTicker.start();
			timeResetter = new Timer(1000 * timeLimitSec, new TimeResetterListener());
			timeResetter.start();
		}
	}
	private void emptyTurnChange() {
		// reset board
		switch (turn) {
		case RenjuBoard.BLACK: turn = RenjuBoard.WHITE; break;
		case RenjuBoard.WHITE: turn = RenjuBoard.BLACK; break;
		}
		clicked			= false;
		clickLegal		= false;
		if (clickedGrid != null) {		
			clickedGrid.click = null;
			clickedGrid.repaint();
			clickedGrid = null;
		}
		// reset timeLabel
		timeLabel.setAndRepaint(0, timeLimitSec);
		// reset turnLabel
		turnLabel.switchAndRepaint();
		// reset placeButton
		placeButton.setEnabled(false);
		// reset time
		timeTicker.restart();
		timeResetter.restart();
	}
	protected void winTask() {
		// should be overridden
		JOptionPane.showConfirmDialog(this, ((turn == RenjuBoard.BLACK)? "BLACK player" : "WHITE player") + " win");
	}
	
	private class Grid extends JPanel {
		public final int x;
		public final int y;
		BufferedImage empty;
		BufferedImage click; // must given each click
		BufferedImage stone; // must given after placement
		
		public Grid(int x, int y, BufferedImage empty) {
			this.x = x;
			this.y = y;
			this.empty = empty;
			click = null;
			stone = null;
//			setLocation((x-1)*50, (y-1)*50);
		}
		
		@Override
		public void paintComponent (Graphics g) {
	      super.paintComponent(g);
	      g.drawImage(empty, 0, 0, this);
	      if (click != null) g.drawImage(click, 0, 0, this);
	      if (stone != null) g.drawImage(stone, 0, 0, this);
	    }
	}
	private class GridListener extends MouseAdapter {
		@Override
		public void mousePressed(MouseEvent e) {
			// test & repaint
			// check clickable
			// check LeftB: make selected | RightB: remove selected
			// turn: 		test based on turn
			// clickedGrid:	reset & set
			// clickLegal: 	??unnecessary
			// placeButton: enable/disable
			if (clickable == true && SwingUtilities.isLeftMouseButton(e)) {
				//reset old grid & repaint
				if (clickedGrid != null) {
					clickedGrid.click = null;
					clickedGrid.repaint();
				}
				Grid g = (Grid) e.getSource();
				// only if the grid doen't have stone
				if (g.stone == null) {
					switch (turn) {
					case RenjuBoard.BLACK:
						if (boardData.testBlack(g.x, g.y) == RenjuBoard.TEST_ILL)
							clickLegal = false;
						else
							clickLegal = true;
						break;
					case RenjuBoard.WHITE: boardData.testWhite(g.x, g.y);
						if (boardData.testWhite(g.x, g.y) == RenjuBoard.TEST_ILL)
							clickLegal = false;
						else
							clickLegal = true;
						break;
					}
					// give grid image & enable/disable placeButton
					if (clickLegal == true) {
						g.click = legal; placeButton.setEnabled(true); 
					} else {
						g.click = illeg; placeButton.setEnabled(false);
					}
					// set new grid & repaint
					clickedGrid = g;
					clickedGrid.repaint();
				}
				else {
					clickedGrid = null;
					placeButton.setEnabled(false);
				}
			}
			else if (clickable == true && SwingUtilities.isRightMouseButton(e)) {
				// reset old grid
				if (clickedGrid != null) {
					clickedGrid.click = null;
					clickedGrid.repaint();
					clickedGrid = null;			// no new grid
				}
				placeButton.setEnabled(false);
			}			
		}
	}
	
	private class TimeLabel extends JLabel {
		public int timeToShow;
		
		public TimeLabel(Color fontColor, Color backColor) {
			if (fontColor != null)
				setForeground(fontColor);
			if (backColor != null)
				setBackground(backColor);
			this.setFont(myfont);
		}
		public void setAndRepaint(int decre, int reset) {
			if (decre != 0 && timeToShow != 0)
				timeToShow -= decre;
			else if (reset != 0)
				timeToShow = reset;
			
			setText("Time: " + Integer.toString(timeToShow));
			repaint();
		}
	}
		
	private class TurnLabel extends JPanel {
		private BufferedImage black;
		private BufferedImage white;
		private BufferedImage plate;
		private byte stone;
		public TurnLabel(BufferedImage black, BufferedImage white, BufferedImage plate) {
			this.black = black; this.white = white; this.plate = plate;
			stone = RenjuBoard.BLACK;
		}
		public void switchAndRepaint() {
			if		(stone == RenjuBoard.BLACK) stone = RenjuBoard.WHITE;
			else if (stone == RenjuBoard.WHITE)	stone = RenjuBoard.BLACK;			
			repaint();
		}
		@Override
		public void paintComponent(Graphics g) {
			super.paintComponent(g);
			// draw plate
			g.drawImage(plate, 0, 0, this);
			// draw stone
			switch (stone) {
			case RenjuBoard.BLACK: g.drawImage(black, 0, 0, this); break;
			case RenjuBoard.WHITE: g.drawImage(white, 0, 0, this); break;
			}
		}
	}
	
	private class PlaceButtonListener implements ActionListener {
		@Override
		public void actionPerformed(ActionEvent e) {
			// check again..
			if (clickLegal == true && clickedGrid != null) {
				// change board status
				byte test = boardData.makeMove(clickedGrid.x, clickedGrid.y, turn);
				
				// win: stop Timer, handle winTask()
				if (test == RenjuBoard.TEST_WIN) {
					if (timeLimitSec != 0) {
						timeTicker.stop();
						timeResetter.stop();
					}
					winTask();
				}
				else {
					// reset current clicked grid
					clickedGrid.click = null;
					// give stone image & change turn
					switch (turn) {
					case RenjuBoard.BLACK: clickedGrid.stone = black; turn = RenjuBoard.WHITE; break;
					case RenjuBoard.WHITE: clickedGrid.stone = white; turn = RenjuBoard.BLACK; break;
					}
					clickedGrid.repaint();
					clickedGrid = null;
				
					// reset timer
					if (timeLimitSec != 0) {
						timeTick = timeLimitSec;
						timeLabel.setAndRepaint(0, timeTick);
						timeTicker.restart();
						timeResetter.restart();
					}
					// switch turn
					turnLabel.switchAndRepaint();
					// disable placeButton
					placeButton.setEnabled(false);
					// restart timer
				}
			}
		}
	}
	
	private class TimeTickerListener implements ActionListener {
		@Override
		public void actionPerformed(ActionEvent e) {
			// decrease timeLabel
			timeTick--;
			timeLabel.setAndRepaint(1, 0);
		}
	}
	
	private class TimeResetterListener implements ActionListener {
		@Override
		public void actionPerformed(ActionEvent arg0) {
			emptyTurnChange();
		}
	}
}