package Board;
// automatically initialized
public class RenjuBoard {
	// must be set public for recursive search
	public byte[][] board = new byte[17][17];
	
	public static final byte BLACK = 0;
	public static final byte WHITE = 1;
	public static final byte EMPTY = 2;
	
	public static final byte TEST_ERR = -1;
	public static final byte TEST_ILL = 0;
	public static final byte TEST_LEG = 1;
	public static final byte TEST_WIN = 2;
	
	// every information about test
	private WData[] wdata = new WData[4];
	
	private static final byte A_BLACK = 0;
	private static final byte A_ILLEG = 1;
	private static final byte A_EMPTY = 2;
	
	private static final byte W_VERT = 0;
	private static final byte W_HORZ = 1;
	private static final byte W_DGNE = 2;
	private static final byte W_DGSE = 3;
	
	private static final byte D_N = 0;
	private static final byte D_NE = 1;
	private static final byte D_E = 2;
	private static final byte D_SE = 3;
	private static final byte D_S = 4;
	private static final byte D_SW = 5;
	private static final byte D_W = 6;
	private static final byte D_NW = 7;
	
	private static final byte T_ILL = 0;
	private static final byte T_CLO = 1;
	private static final byte T_WIN = 2;
	private static final byte T_UN1 = 3;
	private static final byte T_UN2 = 4;
	private static final byte T_UNK = 5;
	private static final byte T_BS3 = 6;
	private static final byte T_BS4 = 7;
	private static final byte T_OP3 = 8;
	private static final byte T_OP4 = 9;
		
	private class BSData {
		int len;
		byte by;
		int x;
		int y;
	}
	private class MSData {
		int len;
		byte by;
		byte[] abs;
		int x;
		int y;
		public MSData() {
			abs = new byte[4];
		}
	}
	private class WData {
		BSData[] bs;
		MSData[] ms;
		byte wresult;
		byte[] mresult = new byte[2];
		public WData() {
			bs = new BSData[2];			
			ms = new MSData[2];
			for (int i = 0; i < 2; i++) {
				bs[i] = new BSData();
				ms[i] = new MSData();
			}
		}
	}
	
	public RenjuBoard() {
		for (int i = 0; i < 17; i++)
			for (int j = 0; j < 17; j++)
				board[i][j] = EMPTY;		
	}
	
	
	public byte makeMove(int x, int y, byte color) {
		
		if (color == BLACK) {
			byte t = testBlack(x, y);
			
			switch(t) {
			case TEST_ILL:
				return t;
				
			case TEST_LEG:				
			case TEST_WIN:
				board[x][y] = color;
				return t;
				
			default:
				break;
			}			
		}
		else if (color == WHITE) {
			byte t = testWhite(x, y);
			
			switch(t) {
			case TEST_ILL:
				return t;
				
			case TEST_LEG:				
			case TEST_WIN:
				board[x][y] = color;
				return t;
				
			default:
				break;
			}			
		}
		
		return TEST_ERR;
	}
	
	public byte testBlack(int x, int y) {
		if (x < 1 ||x > 15 || y < 1 || y > 15)
			return TEST_ILL;
		
		for (int i = 0; i < 4; i++)
			wdata[i] = new WData();
		
		int win = 0, un = 0;
		int i;
		byte wresult;
		
		for (i = 0; i < 4; i++) {			
			wresult = testWay(x, y, (byte)i);			
	        switch (wresult) {
	        case T_ILL:
	            return TEST_ILL;
	        case T_CLO:
	            break;
	        case T_WIN:
	            win++; break;
	        case T_OP3:
	        case T_OP4:
	        case T_UN1:
	        case T_BS3:
	        case T_BS4:
	        case T_UNK:
	            un++; break;
	        case T_UN2:
	            un += 2; break;
	        }
	        wdata[i].wresult = wresult;
	    }
		
		// for quick check
		if (un <= 1) {
			if (win >= 1)
				return TEST_WIN;
			return TEST_LEG;
		}
		
		// recursive check the unsafe
		int o3 = 0, o4 = 0;
		int j, bslen;
		byte mresult;
		RenjuBoard rb = new RenjuBoard();
		copyBoard(board, rb.board);
		rb.board[x][y] = BLACK;
		
		for (i = 0; i < 4; i++) {
			wresult = wdata[i].wresult;
	        switch (wresult) {
	        case T_CLO:
	        case T_WIN:
	            break;

	        case T_BS3:
	            if (rb.testBlack(wdata[i].bs[0].x, wdata[i].bs[0].y) == TEST_ILL || // OR!
	            rb.testBlack(wdata[i].bs[1].x, wdata[i].bs[1].y) == TEST_ILL) {
	                un--;
	            }
	            o3++;
	            break;

	        case T_BS4:
	            if (rb.testBlack(wdata[i].bs[0].x, wdata[i].bs[0].y) == TEST_ILL && // AND!
	            rb.testBlack(wdata[i].bs[1].x, wdata[i].bs[1].y) == TEST_ILL) {
	                un--;
	            }
	            o4++;
	            break;

	        case T_UN1:
	        case T_UN2:
	        case T_UNK:
	            bslen = wdata[i].bs[0].len + wdata[i].bs[1].len + 1;
	            
	            switch (bslen) {
	            case 1:
	            case 2:
	                for (j = 0; j < 2; j++) {
	                    mresult = wdata[i].mresult[j];
	                    switch (mresult) {
	                    case T_CLO: // pass
	                        break;

	                    case T_OP3: // bs || ms
	                        if (rb.testBlack(wdata[i].bs[j].x, wdata[i].bs[j].y) == TEST_ILL ||
	                        rb.testBlack(wdata[i].ms[j].x, wdata[i].ms[j].y) == TEST_ILL) {
	                            un--;
	                        }
	                        else
	                            o3++;
	                        break;

	                    case T_OP4: // bs
	                        if (rb.testBlack(wdata[i].bs[j].x, wdata[i].bs[j].y) == TEST_ILL) {
	                            un--;
	                        }
	                        else
	                            o4++;
	                        break;

	                    default:
	                        return TEST_ERR;
	                    }
	                }
	                break;

	            case 3:
	                for (j = 0; j < 2; j++) {
	                	mresult = wdata[i].mresult[j];
	                    switch (mresult) {
	                    case T_CLO: // pass
	                    case T_BS3: // trivial, OP4 is on the other side 
	                        break;
	                    case T_OP4: // bs
	                        if (rb.testBlack(wdata[i].bs[j].x, wdata[i].bs[j].y) == TEST_ILL) {
	                            un--;
	                        }
	                        else
	                            o4++;
	                        break;

	                    default:
	                        return TEST_ERR;
	                    }
	                }
	                break;

	            case 4:
	                for (j = 0; j < 2; j++) {
	                	mresult = wdata[i].mresult[j];
	                    switch (mresult) {
	                    case T_CLO: // pass
	                    case T_OP4: // ??
	                        break;
	                    case T_BS4: // bs, another side is closed
	                        if (rb.testBlack(wdata[i].bs[j].x, wdata[i].bs[j].y) == TEST_ILL) {
	                            un--;
	                        }
	                        else
	                            o4++;
	                        break;

	                    default:
	                        return TEST_ERR;
	                    }
	                }
	                break;

	            default:
	                return TEST_ERR;
	            }
	            break; // case T_UN1 T_UN2 T_UNK end

	        case T_OP3:
	        case T_OP4:
	            return TEST_ERR;
	        }
	      
	        if (un <= 1) {
	            if (win >= 1)
	                return TEST_WIN;
	            return TEST_LEG;
	        }
	        if (o3 >= 2 || o4 >= 2)
	            return TEST_ILL;        
	        
	    } // for(i) end
		
		return TEST_ERR;
	}
	
	private byte testWay(int x, int y, byte w) {
		byte[] d = new byte[2];
		switch (w) {
	    case W_VERT:    d[0] = D_N;   d[1] = D_S;   break;
	    case W_HORZ:    d[0] = D_W;   d[1] = D_E;   break;
	    case W_DGNE:    d[0] = D_NE;  d[1] = D_SW;  break;
	    case W_DGSE:    d[0] = D_NW;  d[1] = D_SE;  break;
	    }
		
		WData wd = wdata[w];
		BSData[] bs = wd.bs;
		MSData[] ms = wd.ms;
		byte[] mresult = wd.mresult;
		findBSP(x, y, d[0], bs[0]);
		findBSP(x, y, d[1], bs[1]);
		int bslen = bs[0].len + bs[1].len + 1;
		int i;
		
		if (bslen == 5)
			return T_WIN;
		if (bslen >= 6)
			return T_ILL;
		
		// stuck both side
		if (bs[0].by == A_ILLEG && bs[1].by == A_ILLEG)
			return T_CLO;
		
		// examine toward open direction
		for (i = 0; i < 2; i++) {
	        if (bs[i].by == A_EMPTY) {
	            findMSP(bs[i].x, bs[i].y, d[i], ms[i]);
	            mresult[i] = testMargin(bslen, ms[i]);
	        }
	        else
	            mresult[i] = T_CLO;
	    }
		if (mresult[0] == T_CLO && mresult[1] == T_CLO) // both stuck
	        return T_CLO;
	    if (mresult[0] == T_BS3 && mresult[1] == T_BS3) { // BS=3 with no additional MS
	        if (ms[0].abs[0] == A_ILLEG && ms[1].abs[0] == A_ILLEG)
	        	return T_CLO;	// WEBBBEW: closed
	    	return T_BS3;	// otherwise common open3
	    }
	    if (mresult[0] == T_BS4 && mresult[1] == T_BS4) // BS=4 with no additional MS
	        return T_BS4;
	    
	    switch (bslen) {
	    case 1:
	    case 2:
	        if (mresult[0] == T_CLO || mresult[1] == T_CLO) // CLO & OP
	            return T_UN1;
	        if (mresult[0] == T_OP4 && mresult[1] == T_OP4) // OP4 & OP4
	            return T_UN2;
	        return T_UNK; // OP3 & OP3, OP3 & OP4
	    case 3:
	        if (mresult[0] == T_CLO || mresult[1] == T_CLO) // CLO & OP4
	            return T_UN1;  // one T_OP4
	        if (mresult[0] == T_BS3 || mresult[1] == T_BS3) // BS3 & OP4
	            return T_UN2;
	        return T_UNK; // OP4 & OP4
	    case 4:
	        return T_UN1; // CLO & BS4
	    }
	    
	    return TEST_ERR;
	}
	
	private byte testMargin(int bslen, MSData msp) {
		int mslen = bslen + msp.len;	
		int l6pos = 4 - bslen;
		
		// too less black
		if (mslen <= 2)
			return T_CLO;		
		// goes Long6
		if (msp.abs[l6pos] == A_BLACK)
			return T_CLO;
		// no additional black in MS
		if (bslen == mslen)
			return (bslen == 3)? T_BS3 : T_BS4;
		// final touch
		switch (mslen) {
		case 3:
			if (msp.by == A_ILLEG)
				return T_CLO;
		case 4:
			return T_OP4;
		}
		return TEST_ERR;
	}
	
	private void findBSP(int x, int y, byte d, BSData bsp) {
		int n;
	    int ix = x, ixincre = 0, ixbound = 0;
	    int iy = y, iyincre = 0, iybound = 0;
	    byte look;
	    
	    switch (d) {
	    	case D_N:  ixincre = -1; iyincre = 0;  break;
	        case D_NE: ixincre = -1; iyincre = +1; break;
	        case D_E:  ixincre = 0;  iyincre = +1; break;
	        case D_SE: ixincre = +1; iyincre = +1; break;
	        case D_S:  ixincre = +1; iyincre = 0;  break;
	        case D_SW: ixincre = +1; iyincre = -1; break;
	        case D_W:  ixincre = 0;  iyincre = -1; break;
	        case D_NW: ixincre = -1; iyincre = -1; break;
	    }
	    switch (ixincre) {
	        case +1: ixbound = 15; break;
	        case 0:  ixbound = -1; break;
	        case -1: ixbound = 1;  break;
	    }
	    switch (iyincre) {
	        case +1: iybound = 15; break;
	        case 0:  iybound = -1; break;
	        case -1: iybound = 1;  break;
	    }
	    
	    bsp.len = 0;
	    bsp.x = ix;
	    bsp.y = iy;
	    
	    for (n = 0; n < 6; n++) {
	        if (ix == ixbound || iy == iybound){
	            bsp.by = A_ILLEG;
	            break;
	        }        
	        
	        ix += ixincre; iy += iyincre;
	        bsp.x = ix;
	        bsp.y = iy;
	        look = board[ix][iy];
	        
	        if (look == EMPTY) {
	            bsp.by = A_EMPTY;
	                       
	            break;
	        }

	        if (look == WHITE) {
	            bsp.by = A_ILLEG;                
	            break;
	        }

	        bsp.len++;
	        bsp.by = A_BLACK;
	    }
	}
	
	private void findMSP(int x, int y, byte d, MSData msp) {
		int n;
	    int ix = x, ixincre = 0, ixbound = 0;
	    int iy = y, iyincre = 0, iybound = 0;
	    byte look;
	    
	    switch (d) {
	    	case D_N:  ixincre = -1; iyincre = 0;  break;
	        case D_NE: ixincre = -1; iyincre = +1; break;
	        case D_E:  ixincre = 0;  iyincre = +1; break;
	        case D_SE: ixincre = +1; iyincre = +1; break;
	        case D_S:  ixincre = +1; iyincre = 0;  break;
	        case D_SW: ixincre = +1; iyincre = -1; break;
	        case D_W:  ixincre = 0;  iyincre = -1; break;
	        case D_NW: ixincre = -1; iyincre = -1; break;
	    }
	    switch (ixincre) {
	        case +1: ixbound = 15; break;
	        case 0:  ixbound = -1; break;
	        case -1: ixbound = 1;  break;
	    }
	    switch (iyincre) {
	        case +1: iybound = 15; break;
	        case 0:  iybound = -1; break;
	        case -1: iybound = 1;  break;
	    }
	    
	    int lflag = 0;
	    msp.x = ix;
	    msp.y = iy;
	    msp.len = 0;
	    for (n = 0; n < 4; n++) {
	        // check boundary
	        if (ix == ixbound || iy == iybound) {
	            msp.abs[n] = A_ILLEG;

	            if (lflag == 1)
	                continue;
	            msp.x = ix;
	            msp.y = iy;
	            msp.by = A_ILLEG;
	            continue;
	        }

	        // look
	        ix += ixincre;  iy += iyincre;
	        look = board[ix][iy];
	        
	        switch(look) {
	            
	            case EMPTY:
	                msp.abs[n] = A_EMPTY;
	                if (lflag == 1) break;
	                //
	                msp.by = A_EMPTY;
	                msp.x = ix;
	                msp.y = iy;
	                lflag = 1;
	                break;
	                


	            case WHITE:
	                msp.abs[n] = A_ILLEG;
	                if (lflag == 1) break;
	                //
	                msp.by = A_ILLEG;
	                msp.x = ix;
	                msp.y = iy;
	                lflag = 1;
	                break;
	                


	            case BLACK:
	                msp.abs[n] = A_BLACK;
	                if (lflag == 1) break;
	                //
	                msp.x = ix;
	                msp.y = iy;
	                msp.len++;
	                break;

	            default:
	                break;
	        }
	    }
	}
	
	private void copyBoard(byte[][] src, byte[][] dest) {
		for (int i = 0; i < 17; i++)
			for (int j = 0; j < 17; j++)
				dest[i][j] = src[i][j];
	}
	
	public byte testWhite(int x, int y) {
		if (x < 1 || x > 15 || y < 1 || y > 15)
	        return TEST_ILL;
		
		byte[] d = new byte[2];
		int wslen;
	    int i;
	    
	    for (i = 0; i < 4; i++) {
	        switch ((byte)i) {
	        case W_VERT:    d[0] = D_N;   d[1] = D_S;   break;
	        case W_HORZ:    d[0] = D_W;   d[1] = D_E;   break;
	        case W_DGNE:    d[0] = D_NE;  d[1] = D_SW;  break;
	        case W_DGSE:    d[0] = D_NW;  d[1] = D_SE;  break;
	        }
	        wslen = findWSP(x, y, d[0]) + findWSP(x, y, d[1]) + 1;
	        if (wslen >= 5)
	            return TEST_WIN;
	    }
	    
	    return TEST_LEG;
	}
	
	private int findWSP(int x, int y, byte d) {
		int n;
	    int ix = x, ixincre = 0, ixbound = 0;
	    int iy = y, iyincre = 0, iybound = 0;

	    switch (d) {
	        case D_N:  ixincre = -1; iyincre = 0;  break;
	        case D_NE: ixincre = -1; iyincre = +1; break;
	        case D_E:  ixincre = 0;  iyincre = +1; break;
	        case D_SE: ixincre = +1; iyincre = +1; break;
	        case D_S:  ixincre = +1; iyincre = 0;  break;
	        case D_SW: ixincre = +1; iyincre = -1; break;
	        case D_W:  ixincre = 0;  iyincre = -1; break;
	        case D_NW: ixincre = -1; iyincre = -1; break;
	    }
	    switch (ixincre) {
	        case +1: ixbound = 15; break;
	        case 0:  ixbound = -1; break;
	        case -1: ixbound = 1;  break;
	    }
	    switch (iyincre) {
	        case +1: iybound = 15; break;
	        case 0:  iybound = -1; break;
	        case -1: iybound = 1;  break;
	    }

	    int len = 0;
	    byte look;

	    for (n = 0; n < 4; n++) {

	        // check boundary
	        if (ix == ixbound || iy == iybound) {
	            return len;
	        }

	        // look
	        ix += ixincre;  iy += iyincre;
	        look = board[ix][iy];
	        
	        switch(look) {
	        case BLACK:
	        case EMPTY:
	            return len;
	        case WHITE:
	            len++;
	            continue;
	        }
	    }
		return len;
	}
	
}