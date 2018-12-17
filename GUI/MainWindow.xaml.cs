///////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - GUI for Project3HelpWPF                      //
// ver 1.0                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018         //
///////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a WPF-based GUI for Project3HelpWPF demo.  It's 
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
 * ver 1/1 : 07 Aug 2018
 * - fixed bug in DirList_MouseDoubleClick by returning when selectedDir is null
 * ver 1.0 : 30 Mar 2018
 * - first release
 * - Several early prototypes were discussed in class. Those are all superceded
 *   by this package.
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

namespace WpfApp1
{
  public partial class MainWindow : Window
  {
        private Stack<string> pathStack_ = new Stack<string>();
        private Translater translater;
        private CsEndPoint endPoint_;
        private Thread rcvThrd = null;
        private int portnum = 9179;
        private Dictionary<string, Action<CsMessage>> dispatcher_
          = new Dictionary<string, Action<CsMessage>>();

        public MainWindow(int port )
        {
            portnum = port;
            InitializeComponent();
        }

       
        //----< process incoming messages on child thread >----------------

        private void processMessages()
        {
          ThreadStart thrdProc = () => {
            while (true)
            {
              CsMessage msg = translater.getMessage();
              string msgId = msg.value("command");
              if (dispatcher_.ContainsKey(msgId))
                dispatcher_[msgId].Invoke(msg);
            }
          };
          rcvThrd = new Thread(thrdProc);
          rcvThrd.IsBackground = true;
          rcvThrd.Start();
        }
        //----< function dispatched by child thread to main thread >-------

        private void clearDirs()
        {
          DirList.Items.Clear();
        }
        private void clearReqFiles()
        {
            RequestFile.Items.Clear();
        }
        private void clearResFiles()
        {
            TestRes.Items.Clear();
        }
        private void clearFiles()
        {
            FileList.Items.Clear();
        }
        //----< function dispatched by child thread to main thread >-------
        private void insertParent()
        {
            DirList.Items.Insert(0, "..");
        }
        private void addDir(string dir)
        {
          DirList.Items.Add(dir);
        }
        private void addReqFile(string file)
        {
            RequestFile.Items.Add(file);
        }
        private void addResFile(string file)
        {
            TestRes.Items.Add(file);
        }
        private void addFile(string file)
        {
          FileList.Items.Add(file);
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
              clearDirs();
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
                  addDir(dir);
                };
                Dispatcher.Invoke(doDir, new Object[] { enumer.Current.Value });
              }
            }
            Action insertUp = () =>
            {
              insertParent();
            };
            Dispatcher.Invoke(insertUp, new Object[] { });
          };
          addClientProc("getDirs", getDirs);
        }
        //----< load getFiles processing into dispatcher dictionary >------

        private void DispatcherLoadGetFiles()
        {
          Action<CsMessage> getFiles = (CsMessage rcvMsg) =>
          {
            Action clrFiles = () =>
            {
              clearFiles();
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
                  addFile(file);
                };
                Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
              }
            }
          };
          addClientProc("getFiles", getFiles);
        }

        //----< load getFiles processing into dispatcher dictionary >------

        private void DispatcherLoadGetReqFiles()
        {
            Action<CsMessage> getReqFiles = (CsMessage rcvMsg) =>
            {
                Action clrReqFiles = () =>
                {
                    clearReqFiles();
                };
                Dispatcher.Invoke(clrReqFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("file"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            addReqFile(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("getReqFiles", getReqFiles);
        }

        //----< load getFiles processing into dispatcher dictionary >------

        private void DispatcherLoadGetResult()
        {
            Action<CsMessage> getResult = (CsMessage rcvMsg) =>
            {
                Action clrReqFiles = () =>
                {
                    //clearResFiles();
                };
                Dispatcher.Invoke(clrReqFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("result"))
                    {
                        Action<string> doFile = (string res) =>
                        {
                            addResFile(res);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("result", getResult);
        }


        //----< load all dispatcher processing >---------------------------
        private void loadDispatcher()
        {
          DispatcherLoadGetDirs();
          DispatcherLoadGetFiles();
          DispatcherLoadGetReqFiles();
        }
        //----< start Comm, fill window display with dirs and files >------

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
          // start Comm
          endPoint_ = new CsEndPoint();
          endPoint_.machineAddress = "localhost";
          endPoint_.port = portnum;
          translater = new Translater();
          translater.listen(endPoint_);

          // start processing messages
          processMessages();

          // load dispatcher
          loadDispatcher();

          CsEndPoint serverEndPoint = new CsEndPoint();
          serverEndPoint.machineAddress = "localhost";
          serverEndPoint.port = 9180;
          
          PathTextBlock.Text = "SendFile";
          pathStack_.Push("../SendFile");
          CsMessage msg = new CsMessage();
          msg.add("to", CsEndPoint.toString(serverEndPoint));
          msg.add("from", CsEndPoint.toString(endPoint_));
          msg.add("command", "getDirs");
          msg.add("path", pathStack_.Peek());
          translater.postMessage(msg);
          msg.remove("command");
          msg.add("command", "getFiles");
          translater.postMessage(msg);
          test1();
        }
        void test1()
        {
            RequestFile.Items.Add("demo_reqs.tr");
            this.btnDoTests.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
        }
        //----< strip off name of first part of path >---------------------

        private string removeFirstDir(string path)
        {
          string modifiedPath = path;
          int pos = path.IndexOf("/");
          modifiedPath = path.Substring(pos + 1, path.Length - pos - 1);
          return modifiedPath;
        }
        //----< respond to mouse double-click on dir name >----------------

        private void DirList_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
          // build path for selected dir
          string selectedDir = (string)DirList.SelectedItem;
          if (selectedDir == null)
            return;
          string path;
          if(selectedDir == "..")
          {
            if (pathStack_.Count > 1)  // don't pop off "Storage"
              pathStack_.Pop();
            else
              return;
          }
          else
          {
            path = pathStack_.Peek() + "/" + selectedDir;
            pathStack_.Push(path);
          }
          // display path in Dir TextBlcok
          PathTextBlock.Text = removeFirstDir(pathStack_.Peek());
      
          // build message to get dirs and post it
          CsEndPoint serverEndPoint = new CsEndPoint();
          serverEndPoint.machineAddress = "localhost";
          serverEndPoint.port = 9180;
          CsMessage msg = new CsMessage();
          msg.add("to", CsEndPoint.toString(serverEndPoint));
          msg.add("from", CsEndPoint.toString(endPoint_));
          msg.add("command", "getDirs");
          msg.add("path", pathStack_.Peek());
          translater.postMessage(msg);
       
          // build message to get files and post it
          msg.remove("command");
          msg.add("command", "getFiles");
          translater.postMessage(msg);
        }
        private void FileList_MouseDoubleClick(object sender, MouseButtonEventArgs e){
                string selectedItem = FileList.SelectedItem.ToString();
                SelectedList.Items.Add( pathStack_.Peek()+'/'+selectedItem);
                PathTextBlock.Text = pathStack_.Peek()+"/" + selectedItem;
                FileList.Items.Remove(FileList.SelectedItem);
        }
        private void SelectedList_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
                //double click to remove
                SelectedList.Items.Remove(SelectedList.SelectedItem);
        }
    
        //----< first test not completed >---------------------------------
       
        string encode(List<string> strs)
        {
            string result="";
            for (int i = 0; i < strs.Count; i++)
                result += strs[i].Length+ "$" + strs[i];
            return result;
        }
        string getFileName( string fullFilePath)
        {
            int len = fullFilePath.Split('/').Length;
            if ( len<= 1) return fullFilePath;
            return fullFilePath.Split('/')[len-1];
        }
        private void BtnCreateReq_Click(object sender, RoutedEventArgs e)
        {
            // 1. Serialization : length in the front, strings
            // 2. send csmsg to repo, to create request files in client side
            // 3. send csmsg to repo, to send the dll to SaveFile //if they are not exist
            List<string>  dlls=new List<string>();
            CsEndPoint epRepo = new CsEndPoint();
            epRepo.port = 9180;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(epRepo));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "send_files");
            for ( int i=0;i< SelectedList.Items.Count; i++)
            {
                string dll= getFileName(SelectedList.Items[i].ToString());
                dlls.Add(dll);
                msg.attributes["dll"] = dll;
                translater.postMessage(msg);
            }
            msg.remove("command");
            msg.remove("dll");

            //Serialization for dll file names
            string encodedDllNames = encode(dlls);
            msg.attributes["dlls"] = encodedDllNames;
            msg.add("command", "create_reqsts");
            translater.postMessage(msg);
            SelectedList.Items.Clear();
            clearResFiles();
        }

        private void RequestFile_MouseDoubleClick(object sender, RoutedEventArgs e)
        {
            //double click to remove
            RequestFile.Items.Remove(RequestFile.SelectedItem);
        }
        private void BtnDoTests_Click(object sender, RoutedEventArgs e)
        {
            //  1. send csmessage to th, tell th what is the testrequest(.tr) file name
            CsEndPoint epTest= new CsEndPoint();
            epTest.machineAddress = "localhost";
            epTest.port = 9190;
            DispatcherLoadGetResult();
            for ( int i=0;i< RequestFile.Items.Count; i++)
            {
                CsMessage msg = new CsMessage();
                string fileName = RequestFile.Items[i].ToString();
                msg.add("to", CsEndPoint.toString(epTest));
                msg.add("from", CsEndPoint.toString(endPoint_));
                msg.add("command", "request");
                msg.add("name", fileName);
                //msg.add("file", fileName);
                translater.postMessage(msg);
            }
            RequestFile.Items.Clear();
        }
    }
}
