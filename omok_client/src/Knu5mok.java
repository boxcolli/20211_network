import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import Board.*;

public class Knu5mok extends JFrame {
	
	public Knu5mok() {
		// show menu
		//		Multiplay
		//		Localplay
		//		Howtoplay
		//		Credits
		Container cp = getContentPane();
		cp.add(new LocalGameLinked(30));
		
		setDefaultCloseOperation(EXIT_ON_CLOSE);
		setTitle("KnuOmok");
		setSize(1000, 800);
		setVisible(true);
	}

	public static void main(String[] args) {
		new Knu5mok();

	}
	
	private class MainMenu extends JPanel {
		
	}
	
	// TODO: private class MultiGameLinked extends MultiGame

	private class LocalGameLinked extends LocalGame {
		public LocalGameLinked(int sec) {
			super(sec);
		}
		@Override
		protected void winTask() {
			super.winTask();
		}
	}
	
	private class HowTo extends JDialog {
		
	}
}