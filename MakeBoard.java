import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.TreeMap;
import java.util.Random;

public class MakeBoard {

    static Random generator = new Random();
    static Integer numCycles = 0;
    static Integer maxCycles = 1000000;
    static Integer attempts = 0;
    static TreeMap<Integer,ArrayList<Integer>> board;

    public static void main(String[] args) {

        board = new TreeMap<Integer,ArrayList<Integer>>();
        try {

            if(args.length < 1) {
               System.out.println("Please specify a board size on the command line....");
               return;
            }
            Integer boardSize = Integer.valueOf(args[0]);

            while(!generate(boardSize));
            printOutputFiles(boardSize);

         } catch (Exception e) {
            e.printStackTrace();
         }
     }
    public static boolean generate(Integer boardSize) {

      numCycles = 0;
      System.out.println("Attempt # " + attempts++);
      Boolean success = true;

      initBoard(boardSize);
      for(Integer i=0; i < boardSize; i++) { 
          if(!fillRow(i,boardSize)){ 
             success = false; 
             break;
          }
      }
     return success; 
   }
   public static boolean fillRow(Integer rowNum, Integer boardSize) {

       ArrayList<Integer> workingRow = board.get(rowNum);
       Integer lastCol = 0;
       while(workingRow.contains(0)) {

         // Account for the possibility that we could get stuck 
         if(numCycles  > maxCycles) return false;

         Integer colNum = getRand(boardSize);
         Integer value  = getRand(boardSize)+1; // values are 1 based

         // Try N times before backing up...Set N to boardSize^2
         Boolean backup = false;
         for(Integer i=0; i < boardSize*boardSize; i++) {

           numCycles++;

           // Get a value that works for the row
           while(workingRow.contains(value) ) {
              value = getRand(boardSize) + 1;
           }
           Boolean goodValue = true;

           // Check the column
           for(Integer j=0; j < rowNum; j++) {
                ArrayList<Integer> row = board.get(j);
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
                ArrayList<Integer> row =  board.get(boxPosX * boxDim + kk); 
                for ( int yy = 0; yy < boxDim; yy++ ) {
                   // Check the column 
                   if(row.get(boxPosY * boxDim + yy) == value) {
                      goodValue = false;
                    }
                }
             }
          }
          // If we did not find a good value, then back up.
          if(goodValue) {
             workingRow.set(colNum,value);
             board.put(rowNum,workingRow);
             lastCol = colNum;
             break;
          }
       }
       if(backup) {
             workingRow.set(lastCol,0);
             board.put(rowNum,workingRow);
       }
     }
     return true;
   }

   public static void initBoard(Integer boardSize) {

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
    public static void printOutputFiles(Integer boardSize) 
                       throws IOException {

        try {

            // Print a file that contains the puzzle solution
            File out = new File("sudoku_" + boardSize + "_solution.txt");
            BufferedWriter output =  new BufferedWriter(new FileWriter(out));
            for(Integer i=0; i < boardSize; i++) {
               String outLine = board.get(i).toString();
               outLine = outLine.replace(","," ");;
               outLine = outLine.replace("["," ");;
               outLine = outLine.replace("]"," ");;
               output.append(outLine);
               output.newLine();
            }
            System.out.println("The sudoku puzzle solution is in : " + out.toString());
            output.flush();
            output.close();

            // Write out the input board with some percentage of the squares zeroed out
            // This will be the puzzle input board
            out = new File("sudoku_" + boardSize + "_input.txt");
            output =  new BufferedWriter(new FileWriter(out));
            for(Integer i=0; i < boardSize; i++) {
                ArrayList<Integer> row = board.get(i);
                Integer numZeros = getRand(boardSize);
                for(Integer j=0; j< numZeros; j++) {
                    Integer index = getRand(boardSize);
                    row.set(index,0);
                }
                board.put(i,row);
                String outLine = board.get(i).toString();
                outLine = outLine.replace(","," ");;
                outLine = outLine.replace("["," ");;
                outLine = outLine.replace("]"," ");;
                output.append(outLine);
                output.newLine();
            }
            // Write out input board
            System.out.println("The sudoku input puzzle is in : " + out.toString());
            output.flush();
            output.close();
        }
        catch (IOException e) {
          e.printStackTrace();
        }
    }
}
