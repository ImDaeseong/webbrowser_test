
namespace WinFormsChromeBrowser
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.chromiumWebBrowser = new CefSharp.WinForms.ChromiumWebBrowser();
            this.SuspendLayout();
            // 
            // chromiumWebBrowser
            // 
            this.chromiumWebBrowser.ActivateBrowserOnCreation = false;
            this.chromiumWebBrowser.Dock = System.Windows.Forms.DockStyle.Fill;
            this.chromiumWebBrowser.Location = new System.Drawing.Point(0, 0);
            this.chromiumWebBrowser.Margin = new System.Windows.Forms.Padding(10);
            this.chromiumWebBrowser.Name = "chromiumWebBrowser";
            this.chromiumWebBrowser.Size = new System.Drawing.Size(800, 450);
            this.chromiumWebBrowser.TabIndex = 0;
            this.chromiumWebBrowser.LoadError += new System.EventHandler<CefSharp.LoadErrorEventArgs>(this.chromiumWebBrowser_LoadError);
            this.chromiumWebBrowser.LoadingStateChanged += new System.EventHandler<CefSharp.LoadingStateChangedEventArgs>(this.chromiumWebBrowser_LoadingStateChanged);
            this.chromiumWebBrowser.StatusMessage += new System.EventHandler<CefSharp.StatusMessageEventArgs>(this.chromiumWebBrowser_StatusMessage);
            this.chromiumWebBrowser.AddressChanged += new System.EventHandler<CefSharp.AddressChangedEventArgs>(this.chromiumWebBrowser_AddressChanged);
            this.chromiumWebBrowser.TitleChanged += new System.EventHandler<CefSharp.TitleChangedEventArgs>(this.chromiumWebBrowser_TitleChanged);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.chromiumWebBrowser);
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private CefSharp.WinForms.ChromiumWebBrowser chromiumWebBrowser;
    }
}

