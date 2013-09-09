import java.io.*;
import java.util.ArrayList;
import java.util.Map;
import java.util.Set;
import java.util.Vector;
import java.util.TreeMap;
import java.util.TreeSet;
import java.util.HashSet;
import java.util.HashMap;
import java.util.Arrays;
import java.util.Iterator;
import java.util.Stack;
import java.util.Random;

public class MakeBoard {

    static Random generator = new Random();

    public static void main(String[] args) {

        try {

            if(args.length < 1) {
               System.out.println("Please specify a board size on the command line....");
               return;
            }
            Integer boardSize = Integer.valueOf(args[0]);

            File out = new File("sudoku_" + boardSize + "_input.txt");
            BufferedWriter output =  new BufferedWriter(new FileWriter(out));

            TreeMap<Integer,ArrayList<Integer>> board = new TreeMap<Integer,ArrayList<Integer>>();
            initBoard(board,boardSize);
            for(Integer i=0; i < boardSize; i++) { 
               fillRow(board,i,boardSize);
               String outLine = board.get(i).toString();
               outLine = outLine.replace(","," ");;
               outLine = outLine.replace("["," ");;
               outLine = outLine.replace("]"," ");;
               System.out.println(outLine);
            }

            /* 
            output.append("Hello!");
            output.newLine();
            output.flush();
            */

            output.close();
       } catch (Exception e) {
            e.printStackTrace();
       }
   }
   public static void fillRow(TreeMap<Integer,ArrayList<Integer>> board,
                              Integer rowNum, Integer boardSize) {

       Integer maxToFill = Math.round(boardSize);
       Integer numToFill = getRand(maxToFill);

       for(Integer i=0; i < numToFill; i++) {
           ArrayList<Integer> row = board.get(rowNum);
           Integer colNum = getRand(boardSize);
           Integer value = getRand(boardSize);

           // Get a value that works for the row
           while(row.contains(value)) {
              value = getRand(boardSize);
           }
           Boolean goodValue = true;
           // Check the column
           for(Integer j=0; j < rowNum; j++) {
                row = board.get(j);
                if(row.get(colNum) == value) {
                   goodValue = false;
                 }
           }

           // If the row and column are valid, check the box
           if(goodValue) {
             int boxDim  = (int)Math.sqrt(boardSize);
             Integer boxPosX = rowNum/boxDim ;
             Integer boxPosY = colNum/boxDim ;
             for ( int kk = 0; kk < boxDim; kk++) {
                // Get the row for the box
                row =  board.get(boxPosX * boxDim + kk); 
                for ( int yy = 0; yy < boxDim; yy++ ) {
                   // Check the column 
                   if(row.get(boxPosY * boxDim + yy) == value) {
                      goodValue = false;
                    }
                }
             }
          }
          if(goodValue) {
             row = board.get(rowNum);
             row.set(colNum,value);
             board.put(rowNum,row);
             // System.out.println(row.toString());
          }
      }
   }

   public static void initBoard(TreeMap<Integer,ArrayList<Integer>> board,
                                Integer boardSize) {

       for (Integer i=0; i < boardSize; i++) {
          ArrayList <Integer> zeros = new ArrayList<Integer>(); 
          for (Integer j=0; j < boardSize; j++) {
            zeros.add(0);
          }
          board.put(i,zeros);
       }
    }

    public static Integer getRand(Integer max )  {
       if (max == 0) max = 1;
       Integer val = generator.nextInt(max);
       return val;
    }
}

