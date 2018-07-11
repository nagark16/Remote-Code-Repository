/////////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - GUI for RepositoryApp                          //
//                                                                     //
// Author: Naga Rama Krishna, nrchalam@syr.edu                         //
// Reference: Jim Fawcett                                              //
// Application: RepositoryApp                                          //
// Environment: C++ console                                            // 
// Platform: Lenovo T460                                               // 
// Operating System: Windows 10                                        //
/////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a WPF-based GUI for RepositoryApp demo.  It's 
 * responsibilities are to:
 * - Provide a display of directory contents of a remote ServerPrototype.
 * - It provides a subdirectory list and a filelist for the selected directory.
 * - You can navigate into subdirectories by double-clicking on subdirectory
 *   or the parent directory, indicated by the name "..".
 *   
 * Required Files:
 * ---------------
 * Mainwindow.xaml, MainWindow.xaml.cs
 * Translater.dll
 * 
 * Maintenance History:
 * --------------------
 * ver 2.0 : 27th April 2018
 *  - second release
 * ver 1.0 : 6th April 2018
 * 
 */

// Translater has to be statically linked with CommLibWrapper
// - loader can't find Translater.dll dependent CommLibWrapper.dll
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Threading;
using MsgPassingCommunication;
using System.IO;

namespace WpfApp1
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private Stack<string> browsePathStack_ = new Stack<string>();
        private Stack<string> viewFilePathStack_ = new Stack<string>();
        private Stack<string> checkOutPathStack_ = new Stack<string>();
        private Stack<string> viewMDPathStack_ = new Stack<string>();
        private Translater translater;
        private CsEndPoint endPoint_;
        private Thread rcvThrd = null;
        private Boolean fileShowed = false;
        private string path;
        private Dictionary<string, Action<CsMessage>> dispatcher_ 
        = new Dictionary<string, Action<CsMessage>>();

        //----< process incoming messages on child thread >----------------

        private void processMessages()
        {
            ThreadStart thrdProc = () => {
                while (true)
                {
                    CsMessage msg = translater.getMessage();
                    try
                    {
                        string msgId = msg.value("command");
                        if (msgId.Length > 0 && dispatcher_.ContainsKey(msgId))
                            dispatcher_[msgId].Invoke(msg);
                    }catch(Exception e)
                    {
                        Console.WriteLine(e);
                    }
                    
                }
            };
            rcvThrd = new Thread(thrdProc);
            rcvThrd.IsBackground = true;
            rcvThrd.Start();
        }

        //----< function dispatched by child thread to main thread >-------
        private void clearDirs(string cmd)
        {
            if (cmd.Contains("Browse"))
                BrowseDirList.Items.Clear();
            else if (cmd.Contains("ViewFile"))
                ViewDirList.Items.Clear();
            else if (cmd.Contains("CheckOut"))
                CheckOutDirList.Items.Clear();
            else if (cmd.Contains("ViewMetadata"))
                ViewMDDirList.Items.Clear();
        }
        
        //----< function dispatched by child thread to main thread >-------
        private void addDir(string dir, string cmd)
        {
            if(cmd.Contains("Browse"))
                BrowseDirList.Items.Add(dir);
            else if(cmd.Contains("ViewFile"))
                ViewDirList.Items.Add(dir);
            else if (cmd.Contains("CheckOut"))
                CheckOutDirList.Items.Add(dir);
            else if (cmd.Contains("ViewMetadata"))
                ViewMDDirList.Items.Add(dir);
        }
        //----< function dispatched by child thread to main thread >-------

        private void insertParent(string cmd)
        {
            if (cmd.Contains("Browse"))
                BrowseDirList.Items.Insert(0, "..");
            else if (cmd.Contains("ViewFile"))
                ViewDirList.Items.Insert(0, "..");
            else if (cmd.Contains("CheckOut"))
                CheckOutDirList.Items.Insert(0, "..");
            else if (cmd.Contains("ViewMetadata"))
                ViewMDDirList.Items.Insert(0, "..");
        }
        //----< function dispatched by child thread to main thread >-------

        private void clearFiles(string cmd)
        {
            if (cmd.Contains("Browse"))
                BrowseFileList.Items.Clear();
            else if (cmd.Contains("ViewFile"))
                ViewFileList.Items.Clear();
            else if (cmd.Contains("CheckOut"))
                CheckOutFileList.Items.Clear();
            else if (cmd.Contains("ViewMetadata"))
                ViewMetadataFileList.Items.Clear();
        }
        //----< function dispatched by child thread to main thread >-------

        private void addFile(string file, string cmd)
        {
            if (cmd.Contains("Browse"))
                BrowseFileList.Items.Add(file);
            else if (cmd.Contains("ViewFile"))
                ViewFileList.Items.Add(file);
            else if (cmd.Contains("CheckOut"))
                CheckOutFileList.Items.Add(file);
            else if (cmd.Contains("ViewMetadata"))
                ViewMetadataFileList.Items.Add(file);
        }
        //----< add client processing for message with key >---------------

        private void addClientProc(string key, Action<CsMessage> clientProc)
        {
            dispatcher_[key] = clientProc;
        }
        //----< load getDirs processing into dispatcher dictionary >-------

        private void DispatcherLoadGetDirs()
        {
              Action<CsMessage> getDirs = (CsMessage rcvMsg) =>
              {
                  
                  Action clrDirs = () =>
                {
                  clearDirs(rcvMsg.value("command"));
                };
                Dispatcher.Invoke(clrDirs, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                  string key = enumer.Current.Key;
                  if (key.Contains("dir"))
                  {
                    Action<string> doDir = (string dir) =>
                    {
                      addDir(dir, rcvMsg.value("command"));
                        StatusBarText.Text = "Received reply message from server for " + rcvMsg.value("command");
                    };
                    Dispatcher.Invoke(doDir, new Object[] { enumer.Current.Value });
                  }
                }
                Action insertUp = () =>
                {
                  insertParent(rcvMsg.value("command"));
                };
                Dispatcher.Invoke(insertUp, new Object[] { });
              };
              addClientProc("getDirsForBrowse", getDirs);
            addClientProc("getDirsForViewFile", getDirs);
            addClientProc("getDirsForCheckOut", getDirs);
            addClientProc("getDirsForViewMetadata", getDirs);
        }
    //----< load getFiles processing into dispatcher dictionary >------

    private void DispatcherLoadGetFiles()
    {
      Action<CsMessage> getFiles = (CsMessage rcvMsg) =>
      {
          
          Action clrFiles = () =>
        {
          clearFiles(rcvMsg.value("command"));
        };
        Dispatcher.Invoke(clrFiles, new Object[] { });
        var enumer = rcvMsg.attributes.GetEnumerator();
        while (enumer.MoveNext())
        {
          string key = enumer.Current.Key;
          if (key.Contains("file"))
          {
            Action<string> doFile = (string file) =>
            {
              addFile(file, rcvMsg.value("command"));
                StatusBarText.Text = "Received reply message from server for " + rcvMsg.value("command");
            };
            Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
          }
        }
      };
      addClientProc("getFilesForBrowse", getFiles);
            addClientProc("getFilesForViewFile", getFiles);
            addClientProc("getFilesForCheckOut", getFiles);
            addClientProc("getFilesForViewMetadata", getFiles);
        }
        //----< check in processing into dispatcher dictionary >------
        private void DispatcherCheckInFiles()
        {
            Action<CsMessage> checkIn = (CsMessage rcvMsg) =>
            {
                Action<string> doFile = (string file) =>
                {
                    StatusBarText.Text = "Received reply message from server for " + rcvMsg.value("command");
                };
                Dispatcher.Invoke(doFile, new Object[] { ""});
                
            };
            addClientProc("checkInFiles", checkIn);
        }
        //----< check out processing into dispatcher dictionary >------
        private void DispatcherCheckOutFiles()
        {
            Action<CsMessage> checkOut = (CsMessage rcvMsg) =>
            {
                Action<string> doFile = (string file) =>
                {
                    StatusBarText.Text = "Received reply message from server for " + rcvMsg.value("command");
                };
                Dispatcher.Invoke(doFile, new Object[] {"" });
                
            };
            addClientProc("checkOutFiles", checkOut);
        }

        //----< check out processing into dispatcher dictionary >------
        private void DispatcherConnectToServer()
        {
            Action<CsMessage> connectServer = (CsMessage rcvMsg) =>
            {
                Action<string> doFile = (string file) =>
                {
                    StatusBarText.Text = "Successfully connected to server";
                };
                Dispatcher.Invoke(doFile, new Object[] { "" });

            };
            addClientProc("connectToServer", connectServer);
        }

        //----< check out processing into dispatcher dictionary >------
        private void DispatcherServerQuit()
        {
            Action<CsMessage> serverClose = (CsMessage rcvMsg) =>
            {
                Action<string> doFile = (string file) =>
                {
                    StatusBarText.Text = "Server closed";
                };
                Dispatcher.Invoke(doFile, new Object[] { "" });

            };
            addClientProc("serverQuit", serverClose);
        }

        

        //----< check out processing into dispatcher dictionary >------
        private void DispatcherViewFile()
        {
            Action<CsMessage> viewFile = (CsMessage rcvMsg) =>
            {
                Action<string> doFile = (string file) =>
                {
                    StatusBarText.Text = "Successfully received file from server";
                    viewSelectedFile(rcvMsg.value("file"));
                };
                Dispatcher.Invoke(doFile, new Object[] { "" });

            };
            addClientProc("viewFile", viewFile);
        }

        //----< check out processing into dispatcher dictionary >------
        private void DispatcherViewMetadata()
        {
            Action<CsMessage> viewMetadata = (CsMessage rcvMsg) =>
            {
                Action clrFiles = () =>
                {
                    clearMetadataContent();
                };
                Dispatcher.Invoke(clrFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("description"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            addMetadataContent(file);
                            Console.WriteLine(file);
                            StatusBarText.Text = "Received reply message from server for metadata";
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("metadataContent", viewMetadata);
        }

        //----< load all dispatcher processing >---------------------------

        private void loadDispatcher()
        {
            DispatcherLoadGetDirs();
            DispatcherLoadGetFiles();
            DispatcherCheckInFiles();
            DispatcherCheckOutFiles();
            DispatcherConnectToServer();
            DispatcherViewFile();
            DispatcherServerQuit();
            DispatcherViewMetadata();
            DispatcherPackageDescription();
        }
        //----< start Comm, fill window display with dirs and files >------

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            string[] args = System.Environment.GetCommandLineArgs();
            string dir = Directory.GetCurrentDirectory();
            if(dir.Contains("Debug"))
                path = "../codeRepository/remoteRepositoryFiles";
            else
                path = "codeRepository/remoteRepositoryFiles";
            endPoint_ = new CsEndPoint();
            endPoint_.machineAddress = "localhost";
            endPoint_.port = Int32.Parse(args[1]);
            translater = new Translater();
            translater.listen(endPoint_);
            processMessages();
            loadDispatcher();
            ConnectServerIPAddress.Text = "localhost";
            ConnectServerPortNumber.Text = "8080";
            connectToServer("localhost","8080");
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;
            updateCheckInTextBoxForDemo();
            BrowsePathTextBlock.Text = "codeRepository/remoteRepositoryFiles";
            ViewPathTextBlock.Text = "codeRepository/remoteRepositoryFiles";
            CheckOutPathTextBlock.Text = "codeRepository/remoteRepositoryFiles";
            ViewMDPathTextBlock.Text = "codeRepository/remoteRepositoryFiles";
            viewMDPathStack_.Push(path);
            checkOutPathStack_.Push(path);
            browsePathStack_.Push(path);
            viewFilePathStack_.Push(path);
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            initiateMessage(msg);
        }
        //----< helper functin to Window loaded >-------------------------
        private void initiateMessage(CsMessage msg)
        {
            msg.add("command", "getDirsForBrowse");
            msg.add("path", browsePathStack_.Peek());
            translater.postMessage(msg);
            msg.remove("command");
            msg.add("command", "getFilesForBrowse");
            translater.postMessage(msg);
            msg.remove("command");
            msg.add("command", "getFilesForViewFile");
            translater.postMessage(msg);
            msg.remove("command");
            msg.remove("path");
            msg.add("path", viewFilePathStack_.Peek());
            msg.add("command", "getDirsForViewFile");
            translater.postMessage(msg);
            msg.remove("command");
            msg.add("command", "getFilesForCheckOut");
            translater.postMessage(msg);
            msg.remove("command");
            msg.remove("path");
            msg.add("path", checkOutPathStack_.Peek());
            msg.add("command", "getDirsForCheckOut");
            translater.postMessage(msg);
            msg.remove("command");
            msg.add("command", "getFilesForViewMetadata");
            translater.postMessage(msg);
            msg.remove("command");
            msg.remove("path");
            msg.add("path", viewMDPathStack_.Peek());
            msg.add("command", "getDirsForViewMetadata");
            translater.postMessage(msg);
            viewFile("CheckIn.cpp.1");
            viewMetadata("CheckIn.cpp.1");
            packageDescriptionHelper("CheckIn.cpp.1");
        }
    //----< strip off name of first part of path >---------------------

    private string removeFirstDir(string path)
    {
      string modifiedPath = path;
      int pos = path.IndexOf("/");
      modifiedPath = path.Substring(pos + 1, path.Length - pos - 1);
      return modifiedPath;
    }
    //----< respond to mouse double-click on dir name for browse>----------------

    private void DirList_MouseDoubleClick(object sender, MouseButtonEventArgs e)
    {
      // build path for selected dir
      string selectedDir = (string)BrowseDirList.SelectedItem;
      string path;
      if(selectedDir == "..")
      {
        if (browsePathStack_.Count > 1)  // don't pop off "Storage"
          browsePathStack_.Pop();
        else
          return;
      }
      else
      {
        path = browsePathStack_.Peek() + "/" + selectedDir;
        browsePathStack_.Push(path);
      }
            // display path in Dir TextBlcok
            BrowsePathTextBlock.Text = removeFirstDir(browsePathStack_.Peek());
      
      // build message to get dirs and post it
      CsEndPoint serverEndPoint = new CsEndPoint();
      serverEndPoint.machineAddress = "localhost";
      serverEndPoint.port = 8080;
      CsMessage msg = new CsMessage();
      msg.add("to", CsEndPoint.toString(serverEndPoint));
      msg.add("from", CsEndPoint.toString(endPoint_));
      msg.add("command", "getDirsForBrowse");
      msg.add("path", browsePathStack_.Peek());
      translater.postMessage(msg);
      
      // build message to get files and post it
      msg.remove("command");
      msg.add("command", "getFilesForBrowse");
      translater.postMessage(msg);
    }


        //----< respond to mouse double-click on dir name for view file>----------------

        private void DirList_MouseDoubleClickForViewFile(object sender, MouseButtonEventArgs e)
        {
            // build path for selected dir
            string selectedDir = (string)ViewDirList.SelectedItem;
            string path;
            if (selectedDir == "..")
            {
                if (viewFilePathStack_.Count > 1)  // don't pop off "Storage"
                    viewFilePathStack_.Pop();
                else
                    return;
            }
            else
            {
                path = viewFilePathStack_.Peek() + "/" + selectedDir;
                viewFilePathStack_.Push(path);
            }
            // display path in Dir TextBlcok
            ViewPathTextBlock.Text = removeFirstDir(viewFilePathStack_.Peek());

            // build message to get dirs and post it
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getDirsForViewFile");
            msg.add("path", viewFilePathStack_.Peek());
            translater.postMessage(msg);

            // build message to get files and post it
            msg.remove("command");
            msg.add("command", "getFilesForViewFile");
            translater.postMessage(msg);
        }

        //----< respond to mouse double-click on dir name for view file>----------------

        private void DirList_MouseDoubleClickForMetadata(object sender, MouseButtonEventArgs e)
        {
            // build path for selected dir
            string selectedDir = (string)ViewMDDirList.SelectedItem;
            string path;
            if (selectedDir == "..")
            {
                if (viewMDPathStack_.Count > 1)  // don't pop off "Storage"
                    viewMDPathStack_.Pop();
                else
                    return;
            }
            else
            {
                path = viewMDPathStack_.Peek() + "/" + selectedDir;
                viewMDPathStack_.Push(path);
            }
            // display path in Dir TextBlcok
            ViewPathTextBlock.Text = removeFirstDir(viewMDPathStack_.Peek());

            // build message to get dirs and post it
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getDirsForViewMetadata");
            msg.add("path", viewMDPathStack_.Peek());
            translater.postMessage(msg);

            // build message to get files and post it
            msg.remove("command");
            msg.add("command", "getFilesForViewMetadata");
            translater.postMessage(msg);
        }

        //----< respond to mouse double-click on dir name for check out>----------------

        private void DirList_MouseDoubleClickCheckOut(object sender, MouseButtonEventArgs e)
        {
            // build path for selected dir
            string selectedDir = (string)CheckOutDirList.SelectedItem;
            string path;
            if (selectedDir == "..")
            {
                if (checkOutPathStack_.Count > 1)  // don't pop off "Storage"
                    checkOutPathStack_.Pop();
                else
                    return;
            }
            else
            {
                path = checkOutPathStack_.Peek() + "/" + selectedDir;
                checkOutPathStack_.Push(path);
            }
            // display path in Dir TextBlcok
            CheckOutPathTextBlock.Text = removeFirstDir(checkOutPathStack_.Peek());

            // build message to get dirs and post it
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getDirsForCheckOut");
            msg.add("path", checkOutPathStack_.Peek());
            translater.postMessage(msg);

            // build message to get files and post it
            msg.remove("command");
            msg.add("command", "getFilesForCheckOut");
            translater.postMessage(msg);
        }
        //----< first test not completed >---------------------------------

        void test1()
    {
      MouseButtonEventArgs e = new MouseButtonEventArgs(null, 0, 0);
            BrowseDirList.SelectedIndex = 2;
      DirList_MouseDoubleClick(this, e);
    }



        //----< respond to select button click in CheckIn tab>----------------
        private void CheckInFileFindButtonAction(object sender, RoutedEventArgs e)
        {

            var ofd = new Microsoft.Win32.OpenFileDialog();
            string dir = Directory.GetCurrentDirectory();
            int index = dir.IndexOf("RepositoryApp");
            dir = dir.Substring(0, index);
            dir = dir + @"RepositoryApp\codeRepository\localClientFiles";

            ofd.InitialDirectory = dir;
            var result = ofd.ShowDialog();
            if (result == false) return;
            updateCheckInTextBox(ofd.FileName);
        }
        //----< Helper function to update checkinfile text box for demo>-----------------
        private void updateCheckInTextBoxForDemo()
        {
            CheckOutHelper("Browse.cpp.1", false);
            string dir = Directory.GetCurrentDirectory();
            String file;
            if (dir.Contains("Debug"))
                file = System.IO.Path.GetFullPath("../../../../codeRepository/localClientFiles/Browse.cpp");
            else
                file = System.IO.Path.GetFullPath("codeRepository/localClientFiles/Browse.cpp");
            updateCheckInTextBox(file);
            demonstrateCheckInProcess();
            CheckOutHelper("CheckIn.h.1", true);
        }
        //----< Helper function to update checkinfile text box>-------------------------
        private void updateCheckInTextBox(string filename)
        {
            CheckInFileTextBox.Foreground = new SolidColorBrush(Colors.Black);
            CheckInFileTextBox.Text = getRelativeFilepath(filename);
            AuthorTextBox.Text = "naga";
            CategoryTextBox.Text = "NoSqlDB";
            DescriptionTextBox.Text = "This is description";
            updateCheckInFileList(filename);
        }
        //----< Helper function for relative path >-------------------------
        private string getRelativeFilepath(string fullFilePath)
        {
            return fullFilePath.Substring(fullFilePath.IndexOf("codeRepository"));
        }
        //----< Helper function to update check in file list>-------------------------
        private void updateCheckInFileList(string filePath)
        {
            int index = filePath.LastIndexOf('\\');
            CheckInFileList.Items.Clear();
            string subPath = filePath.Substring(0, index);
            string[] tempFiles = Directory.GetFiles(subPath);
            for (int i = 0; i < tempFiles.Count(); ++i)
                if(tempFiles[i] != filePath)
                    CheckInFileList.Items.Add(getRelativeFilepath(tempFiles[i]));
            
        }
        //----< Helper function to file select button>-------------------------
        private void FileSelect(object sender, RoutedEventArgs e)
        {
            foreach(var file in BrowseFileList.SelectedItems)
            {
                Console.WriteLine(file);
            }
        }
        //----< Helper function for check in files button click>-------------------------
        private void CheckInFilesAction(object sender, RoutedEventArgs e)
        {
            int pos = CheckInFileTextBox.Text.LastIndexOf("\\");
            string path = CheckInFileTextBox.Text.Substring(0, pos);
            string parent = CheckInFileTextBox.Text.Substring(pos + 1);
            string dep2 = "";
            foreach (string dep in CheckInFileList.SelectedItems)
            {
                CheckInFilesHelper(dep.Substring(dep.LastIndexOf("\\") + 1), path, AuthorTextBox.Text, CategoryTextBox.Text, DescriptionTextBox.Text, CheckInChecked.IsChecked ?? false, "");
                dep2 = dep;
            }
            dep2 = dep2.Substring(dep2.LastIndexOf("\\")+1);
            CheckInFilesHelper(parent, path, AuthorTextBox.Text, CategoryTextBox.Text, DescriptionTextBox.Text, CheckInChecked.IsChecked ?? false, dep2);
        }
        //----< Helper function to demonstrate checkin process>-------------------------
        private void demonstrateCheckInProcess()
        {
            CheckInFilesHelper("Browse.cpp", "codeRepository/localClientFiles", "naga", "repositoryCore", "Trying to CheckIn open file", false,"");
            CheckInFilesHelper("Browse.cpp", "codeRepository/localClientFiles", "rama", "repositoryCore", "Trying to CheckIn same \"Browse.cpp\" file as a different user", false,"");
            CheckInFilesHelper("Browse.cpp", "codeRepository/localClientFiles", "naga", "repositoryCore", "Trying to close already checkedin file", true,"");
            CheckInFilesHelper("Browse.cpp", "codeRepository/localClientFiles", "naga", "repositoryCore", "Trying to checkin \"Browse.cpp\" version 2 after adding a child with closed status, which is not checked in yet", true, "XmlDocument.cpp");
            CheckInFilesHelper("XmlDocument.cpp", "codeRepository/localClientFiles", "naga", "repositoryCore", "Trying to checkin child \"XmlDocument.cpp\" with closed status", true,"");
            CheckInFilesHelper("Browse.cpp", "codeRepository/localClientFiles", "naga", "repositoryCore", "Trying to checkin \"Browse.cpp\" after checking in child", true, "XmlDocument.cpp");
            CheckInFilesHelper("CheckIn.cpp", "codeRepository/localClientFiles", "naga", "repositoryCore", "Trying to checkin \"CheckIn.cpp\" with open status", false, "CheckOut.cpp");
            CheckInFilesHelper("CheckOut.cpp", "codeRepository/localClientFiles", "naga", "repositoryCore", "Trying to checkin \"CheckOut.cpp\" with open status", false, "CheckIn.cpp");
            CheckInFilesHelper("CheckIn.cpp", "codeRepository/localClientFiles", "naga", "repositoryCore", "Demonstrating cyclic dependencies", true, "CheckOut.cpp");
        }
        //----< Helper function for check in>-------------------------
        private void CheckInFilesHelper(string filename, string path, string author, string categories, string description, bool isClosed,string dependencies)
        {
            copyFile(filename);
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;

            CsMessage msg2 = new CsMessage();
            msg2.add("to", CsEndPoint.toString(serverEndPoint));
            msg2.add("from", CsEndPoint.toString(endPoint_));
            msg2.add("command", "checkInFiles");
            msg2.add("files", filename);
            msg2.add("path", path);
            msg2.add("description", description);
            msg2.add("categories", categories);
            msg2.add("dependencies", dependencies);
            msg2.add("name", author);
            msg2.add("isClosed", isClosed.ToString());
            translater.postMessage(msg2);
            
        }

        private void copyFile(string filename)
        {
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "checkIn");
            msg.add("file", filename+":");
            translater.postMessage(msg);
        }

        //----< Helper function for check out button click>-------------------------
        private void CheckOutFilesAction(object sender, RoutedEventArgs e)
        {
            string dependencies = "";
            foreach (string dep in CheckOutFileList.SelectedItems)
                dependencies += dep ;
            CheckOutHelper(dependencies, CheckOutChecked.IsChecked ?? false);
        }

        //----< Helper function for check out>-------------------------
        private void CheckOutHelper(string dependencies,bool isChecked)
        {
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "checkOutFiles");
            msg.add("checkOutFile", dependencies);
            if (isChecked)
                msg.add("isChecked", "TRUE");
            else
                msg.add("isChecked", "FALSE");
            translater.postMessage(msg);
        }
        //----< Helper function server connection button>-------------------------
        private void ServerConnectAction(object sender, RoutedEventArgs e)
        {
            connectToServer(ConnectServerIPAddress.Text, ConnectServerPortNumber.Text);
        }

        //----< Helper function server connection button>-------------------------
        private void connectToServer(string ipadd, string port)
        {
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = ipadd;
            serverEndPoint.port = Int32.Parse(port);
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "connectToServer");
            translater.postMessage(msg);
        }
        //----< Helper function for view file button click>-------------------------
        private void ViewFileDoubleClickAction(object sender, RoutedEventArgs e)
        {
            viewFile((string)ViewFileList.SelectedItem);
        }
        //----< Helper function for view file button click>-------------------------
        private void viewFile(string filename)
        {
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;
            fileShowed = true;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "viewFile");
            msg.add("ViewFileName", filename);
            msg.add("location", @"..\codeRepository\localClientFiles\ViewFile");
            Console.WriteLine("sending message to server");
            translater.postMessage(msg);
        }
        //----< Helper function for view file return message>-------------------------
        private void viewSelectedFile(string file)
        {
            file = file.Substring(0, file.Length - 1);
            Console.WriteLine(file);
            if (fileShowed)
            {
                Console.WriteLine("about to display files");
                fileShowed = false;
                System.Windows.Application.Current.Dispatcher.Invoke((Action)delegate
                {
                    ViewFile viewFile = new ViewFile();
                    string dir = Directory.GetCurrentDirectory();
                    String pathTemp;
                    if (dir.Contains("Debug"))
                        pathTemp = System.IO.Path.GetFullPath("../../../../codeRepository/localClientFiles/" + file);
                    else
                        pathTemp = System.IO.Path.GetFullPath("codeRepository/localClientFiles/" + file);
                    Paragraph paragraph = new Paragraph();
                    paragraph.Inlines.Add(new Run(File.ReadAllText(pathTemp)));
                    viewFile.codeview.Blocks.Add(paragraph);
                    viewFile.Show();
                });
            }
        }
        //----< Helper function for view metadata button click>-------------------------
        private void ViewMetadataAction(object sender, RoutedEventArgs e)
        {
            if(ViewMetadataFileList.SelectedItem != null)
                viewMetadata((string)ViewMetadataFileList.SelectedItem);
        }
        //----< Helper function for view metadata button click>-------------------------
        private void viewMetadata(string file)
        {
            Console.WriteLine("view metadata " + file);
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;
            fileShowed = true;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "metadataContent");
            msg.add("ViewFileName", file);
            translater.postMessage(msg);
        }
        //----< function to get package description action>------
        private void GetPackageDescriptionAction(object sender, RoutedEventArgs e)
        {
            if(BrowseFileList.SelectedItem != null)
                packageDescriptionHelper((string)BrowseFileList.SelectedItem);
        }
        //----< helper function to get package description >------
        private void packageDescriptionHelper(string filename)
        {
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;
            fileShowed = true;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "browseDescription");
            msg.add("fileName", filename);
            translater.postMessage(msg);
        }

        //----< check out processing into dispatcher dictionary >------
        private void DispatcherPackageDescription()
        {
            Action<CsMessage> packageDescription = (CsMessage rcvMsg) =>
            {

                Action clrFiles = () =>
                {
                    clearDescription();
                };
                Dispatcher.Invoke(clrFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("description"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            addDescription(file);
                            Console.WriteLine(file);
                            StatusBarText.Text = "Received reply message from server for package description";
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            };

            addClientProc("browseDescription", packageDescription);
        }

        //----< function dispatched by child thread to main thread >-------

        private void clearDescription()
        {
            BrowsePackageDescription.Items.Clear();
        }
        //----< function dispatched by child thread to main thread >-------

        private void addDescription(string description)
        {
            BrowsePackageDescription.Items.Add(description);
        }

        //----< function dispatched by child thread to main thread >-------

        private void clearMetadataContent()
        {
            MetadataContent.Items.Clear();
        }
        //----< function dispatched by child thread to main thread >-------

        private void addMetadataContent(string description)
        {
            MetadataContent.Items.Add(description);
        }
    }
}
