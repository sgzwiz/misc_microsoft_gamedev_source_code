namespace PhoenixEditor
{
   partial class TabletSettings
   {
      /// <summary>
      /// Required designer variable.
      /// </summary>
      private System.ComponentModel.IContainer components = null;

      /// <summary>
      /// Clean up any resources being used.
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
      /// Required method for Designer support - do not modify
      /// the contents of this method with the code editor.
      /// </summary>
      private void InitializeComponent()
      {
         this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel();
         this.SuspendLayout();
         // 
         // flowLayoutPanel1
         // 
         this.flowLayoutPanel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                     | System.Windows.Forms.AnchorStyles.Left)
                     | System.Windows.Forms.AnchorStyles.Right)));
         this.flowLayoutPanel1.AutoScroll = true;
         this.flowLayoutPanel1.Location = new System.Drawing.Point(3, 3);
         this.flowLayoutPanel1.Name = "flowLayoutPanel1";
         this.flowLayoutPanel1.Size = new System.Drawing.Size(640, 403);
         this.flowLayoutPanel1.TabIndex = 0;
         // 
         // TabletSettings
         // 
         this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
         this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
         this.Controls.Add(this.flowLayoutPanel1);
         this.FloatingWindowBounds = new System.Drawing.Rectangle(0, 0, 646, 409);
         this.Key = "TabletSettings";
         this.Name = "TabletSettings";
         this.Size = new System.Drawing.Size(646, 409);
         this.Text = "TabletSettings";
         this.ResumeLayout(false);

      }

      #endregion

      private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
   }
}