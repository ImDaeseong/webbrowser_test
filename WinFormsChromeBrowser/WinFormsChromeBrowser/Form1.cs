using System;
using System.Windows.Forms;
using CefSharp;
using CefSharp.WinForms;

namespace WinFormsChromeBrowser
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            //초기화
            CefSettings settings = new CefSettings();
            //CefSharpSettings.LegacyJavascriptBindingEnabled = true;
            //settings.RemoteDebuggingPort = 8088;
            //settings.CefCommandLineArgs.Add("disable-usb-keyboard-detect", "1");
            //settings.CefCommandLineArgs.Add("persist_session_cookies", "1");
            //settings.CefCommandLineArgs.Add("disable-gpu-vsync", "1");
            //settings.CefCommandLineArgs.Add("disable-gpu", "1");
            //settings.CefCommandLineArgs.Add("enable-media-stream", "1");
            Cef.Initialize(settings);         
        }

        private void Form1_Load(object sender, System.EventArgs e)
        {
            chromiumWebBrowser.Load("https://www.naver.com");
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            Cef.Shutdown();
        }

        private void chromiumWebBrowser_AddressChanged(object sender, CefSharp.AddressChangedEventArgs e)
        {
            this.Invoke(new MethodInvoker(() =>
            {
                string sMsg = string.Format("chromiumWebBrowser_AddressChanged :{0} ", e.Address);
                Console.WriteLine(sMsg);
            }));            
        }

        private void chromiumWebBrowser_TitleChanged(object sender, CefSharp.TitleChangedEventArgs e)
        {
            this.Invoke(new MethodInvoker(() =>
            {
                string sMsg = string.Format("chromiumWebBrowser_TitleChanged :{0} ", e.Title);
                Console.WriteLine(sMsg);
            }));            
        }

        private void chromiumWebBrowser_StatusMessage(object sender, CefSharp.StatusMessageEventArgs e)
        {
            this.Invoke(new MethodInvoker(() =>
            {
                string sMsg = string.Format("chromiumWebBrowser_StatusMessage :{0} ", e.Value);
                Console.WriteLine(sMsg);
            }));
        }

        private void chromiumWebBrowser_LoadError(object sender, LoadErrorEventArgs e)
        {
            this.Invoke(new MethodInvoker(() =>
            {
                string sMsg = string.Format("chromiumWebBrowser_LoadError :{0} ", e.ErrorText);
                Console.WriteLine(sMsg);
            }));
        }
    }
}
