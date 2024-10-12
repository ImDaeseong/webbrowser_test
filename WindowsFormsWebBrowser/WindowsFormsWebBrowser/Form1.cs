using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsWebBrowser
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Navigate(string sUrl)
        {
            if (!string.IsNullOrEmpty(sUrl) && !sUrl.StartsWith("http://") && !sUrl.StartsWith("https://"))
            {
                sUrl = "https://" + sUrl;
            }
            webBrowser.Navigate(sUrl);
        }

        private void GoBack()
        {
            if (webBrowser.CanGoBack)
                webBrowser.GoBack();
        }

        private void GoForward()
        {
            if (webBrowser.CanGoForward)
                webBrowser.GoForward();
        }


        private void Form1_Load(object sender, EventArgs e)
        {
            Navigate("https://www.naver.com");
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {

        }

        private void webBrowser_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
        {
            string sTitle = webBrowser.DocumentTitle;
            string sAddress = webBrowser.Url?.ToString();
            bool bBack = webBrowser.CanGoBack;
            bool bForward = webBrowser.CanGoForward;

            Console.WriteLine(sTitle);
            Console.WriteLine(sAddress);
            Console.WriteLine(bBack);
            Console.WriteLine(bForward);
        }
    }
}
