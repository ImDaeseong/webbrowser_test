using System;
using Microsoft.Web.WebView2.Core;
using Microsoft.Web.WebView2.WinForms;
using System.Windows.Forms;

namespace WinFormsBrowser
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            if (webView != null)
            {
                webView.Source = new Uri("https://www.naver.com");
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (webView != null)
            {
                webView.Dispose(); 
            }
        }

        private void webView_NavigationStarting(object sender, Microsoft.Web.WebView2.Core.CoreWebView2NavigationStartingEventArgs e)
        {
            Console.WriteLine("webView_NavigationStarting");
        }

        private void webView_NavigationCompleted(object sender, Microsoft.Web.WebView2.Core.CoreWebView2NavigationCompletedEventArgs e)
        {
            Console.WriteLine("webView_NavigationCompleted");
        }
    }
}
