/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/GUIForms/JFrame.java to edit this template
 */
package pl.polsl.SKOCZYLAS.textfileproject;

import java.io.File;
import java.util.Optional;
import java.util.Scanner;
import pl.polsl.SKOCZYLAS.textfileproject.Controller.Controller;
import pl.polsl.SKOCZYLAS.textfileproject.Model.ModelFile;
import pl.polsl.SKOCZYLAS.textfileproject.Model.ModelFileStatus;
import pl.polsl.SKOCZYLAS.textfileproject.Viewer.Viewer;

/**
 *
 * @author Szymon Skoczylas
 * @version 1.3
 */
public class TextfileProjectSwing extends javax.swing.JFrame {
      /**
       * Global variables
       */  
    class Global{
        public static final Controller controller = new Controller();
        public static final ModelFile modelFile = new ModelFile();
        public static final ModelFileStatus modelFileStatus = new ModelFileStatus();
        public static final Viewer view = new Viewer(); 
    }
        
    /**
     * Creates new form TextfileProjectSwing
     */
    public TextfileProjectSwing() {
        initComponents();
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">                          
    private void initComponents() {

        jButton1 = new javax.swing.JButton();
        jButton2 = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

        jButton1.setText("similarity of files");
        jButton1.setActionCommand("generate");
        jButton1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton1ActionPerformed(evt);
            }
        });

        jButton2.setText("show file with more chars");
        jButton2.setActionCommand("moreChars");
        jButton2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton2ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(35, 35, 35)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(jButton2, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jButton1, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addContainerGap(200, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addContainerGap(145, Short.MAX_VALUE)
                .addComponent(jButton2, javax.swing.GroupLayout.PREFERRED_SIZE, 36, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(28, 28, 28)
                .addComponent(jButton1, javax.swing.GroupLayout.PREFERRED_SIZE, 35, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(56, 56, 56))
        );

        pack();
    }// </editor-fold>                        

    private void jButton1ActionPerformed(java.awt.event.ActionEvent evt) {                                         
        Integer percentegeOfSimilarity = Global.modelFile.Comparison(Global.controller.getFiles().get(0),Global.controller.getFiles().get(1));
        String textToDisplay = "Similarity of files: " + percentegeOfSimilarity.toString() +"%";
        Global.view.showInPane(this, textToDisplay);
    }                                        

    private void jButton2ActionPerformed(java.awt.event.ActionEvent evt) {                                         
        Optional<FileStatus> fileWithMoreChars = Global.controller.getFileWithMostChars();
        String textToDisplay = "File with more chars: " + fileWithMoreChars.get().getName();
        Global.view.showInPane(this, textToDisplay);
    }                                        

    /**
    * The main method of the program.
    *
    * @param args An array of command-line arguments. It should contain two elements:
     *             - The file path of the first file.
    *             - The file path of the second file.
    */
    public static void main(String args[]) {
        /* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(TextfileProjectSwing.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(TextfileProjectSwing.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(TextfileProjectSwing.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(TextfileProjectSwing.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>
              
        //Initialize the files
        if(args.length == 2)
        {
            Global.controller.newFile(args[0]);
            Global.controller.newFile(args[1]);
        }
        else
        {
            Scanner scanner = new Scanner(System.in);
            Global.view.show("1st filepath:");
            String tmp;
            tmp = scanner.next();
            Global.controller.newFile(tmp);
            Global.view.show("2nd filepath:");
            tmp = scanner.next();
            Global.controller.newFile(tmp);
        }    
        File file1 = Global.controller.getFiles().get(0).getFile();
        File file2 = Global.controller.getFiles().get(1).getFile();
        Global.controller.getFiles().get(0).setNumberOfChars(Global.modelFileStatus.countChars(file1));
        Global.controller.getFiles().get(1).setNumberOfChars(Global.modelFileStatus.countChars(file2));
        //End of initialization
        
        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new TextfileProjectSwing().setVisible(true);
            }
        });
    }

    // Variables declaration - do not modify                     
    private javax.swing.JButton jButton1;
    private javax.swing.JButton jButton2;
    // End of variables declaration                   
}
