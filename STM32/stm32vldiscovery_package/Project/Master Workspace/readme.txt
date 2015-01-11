 /**
  @page Toolchains Master workspace Readme file
  
  @verbatim
 ******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
 * @file     readme.txt
 * @author   MCD Application Team
 * @version  V1.0.0
 * @date     09/13/2010
 * @brief    How to use workspace project with MDK-ARM, EWARM and TrueSTUDIO 
             toolchains.
 ********************************************************************************
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 * AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
 * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
 * CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
 * INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *******************************************************************************
 @endverbatim
 
@par Description

This readme file describes how to use workspace project with MDK-ARM, EWARM 
and TrueSTUDIO toolchains. 

@par Directory contents 

  - MDK-ARM/Value_Line_Discovery.uvmpw  Preconfigured MDK-ARM worksapce grouping 
                                        all examples and demo projects
  - EWARMv5/Value_Line_Discovery.eww    Preconfigured EWARM worksapce grouping 
                                        all examples and demo projects
  - TrueSTUDIO                          TrueSTUDIO worksapce grouping all examples
                                        and demo projects
   
@par How to use it 

In order to build/debug one project from the master workspace, you have to do the
following:
  
 - EWARM (v5.50.5) 
    - Open the Value_Line_Discovery.eww workspace
    - In the workspace explorer select the project you want to execute
    - Right Click on the selected project and choose "Set As Active" option
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 + MDK-ARM (v4.12) 
    - Open the Value_Line_Discovery.uvmpw Workspace
    - In the Project explorer select the project you want to execute  
    - Right Click on the selected project and choose "Set As Active Project"
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)

+ TrueSTUDIO (v1.4.0)
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace 
      directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the "FIRMWARE/Project" folder, select the examples project you want 
	    to load from the project list and click Finish.  
    - Under Windows->Preferences->General->Workspace->Linked Resources, add 
      a variable path named "CurPath"  which points to the "FIRMWARE" folder.      
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Select the project in the "Project explorer" window then click 
      Run->Debug (F11)

    
 * <h3><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h3>
 */  
 
 
