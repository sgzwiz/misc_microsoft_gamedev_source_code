﻿namespace RemoteMemory
{
   partial class fastTimeline
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

      #region Component Designer generated code

      /// <summary> 
      /// Required method for Designer support - do not modify 
      /// the contents of this method with the code editor.
      /// </summary>
      private void InitializeComponent()
      {
         this.SuspendLayout();
         // 
         // fastTimeline
         // 
         this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
         this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
         this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(63)))), ((int)(((byte)(63)))), ((int)(((byte)(63)))));
         this.Name = "fastTimeline";
         this.Size = new System.Drawing.Size(430, 150);
         this.MouseLeave += new System.EventHandler(this.fastTimeline_MouseLeave);
         this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.fastTimeline_MouseMove);
         this.MouseEnter += new System.EventHandler(this.fastTimeline_MouseEnter);
         this.ResumeLayout(false);

      }

      #endregion
   }
}
