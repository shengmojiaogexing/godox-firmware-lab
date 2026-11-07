import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15
import Qt.labs.platform 1.1



ApplicationWindow
{
	id:rootWindow
	width: 575
	height: 350
	maximumWidth:575
	maximumHeight: 350
	minimumWidth: 575
	minimumHeight: 350
	visible: true
	title: qsTr("Godox固件升级软件")

	property url logoPath: ""
	property url usbIconOn: ""
	property url usbIconOff: ""
	property string help_website: "www.godox.com"


    property bool isdownJsonFile: false      //是否正在下载json文件



    function delay(func, ms)
    {
        var timer = Qt.createQmlObject(
            'import QtQuick 2.15; Timer { interval: ' + ms + '; repeat: false; running: true }',
            rootWindow,  // 放置在 root 对象下
            "dynamicTimer"
        );
        timer.triggered.connect(func);
    }


	//信号
	signal notifyStartUpdate(int Event)

	Component.onCompleted:
	{
		notifyStartUpdate.connect(backend.fromUiEevent)

		logoPath = "file:///" + appdir + "/" + "Config/logo.png"
		usbIconOn = "file:///" + appdir + "/" + "Config/ON.png"
		usbIconOff = "file:///" + appdir + "/" + "Config/OFF.png"
	}

	//退出事件
	onClosing:
	{
		   //do something
		backend.windowsClose();
		close.accepted = true;
	}

	//客户区
	Rectangle
	{
		anchors.fill: parent
		color: Qt.rgba(240/255 , 240/255 , 240/255 , 1.0)

		//左Logo区
		Rectangle
		{
			id:rec_logo
			width: 200
			height: parent.height
			color: "transparent"
			anchors.top: parent.top
			anchors.left: parent.left

			Image
			{
				id: img_logo
				anchors.fill: parent
				anchors.margins: 10
				source: logoPath
			}
		}

		//右边操作区
		Rectangle
		{
			id:rec_Ctrl
			height: parent.height
			//color: "blue"
			color: "transparent"
			anchors.top: parent.top
			anchors.left: rec_logo.right
			anchors.right: parent.right


			//顶部控制显示
			Item
			{
				id:item_top
				anchors.top: parent.top
				anchors.topMargin: 23
				height: 40
				Row
				{
					anchors.topMargin: 23
                    spacing: 5
					Image
					{
						id: img_ConnectStatus
						anchors.top: parent.top
						width: 91
						height: 25
						source: usbIconOff
					}
                    Button
                    {
                        id:bt_netFile
                        anchors.top: parent.top
                        width: 80
                        height: 25
                        text: "网络固件"
                        font.pixelSize: 14
                        font.family: "Arial"

                        // 设置按钮文本
                        contentItem: Text
                        {
                            text: bt_netFile.text
                            font: bt_netFile.font
                            color: "black"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }
                        onClicked:
                        {
                            //加载json文件
                            isdownJsonFile = true;
                            backend.qml_LoadJsonFile();
                            isdownJsonFile = false;


                            if(backend.jsonFileIsExists())
                            {
                                //打开弹窗
                                netFileDialog.visible = true;

                                //清除原来输入的内容
                                netFileDialog.searchInputText = "";

                                //如果读到V2.0的设备ID,做一次Search动做
                                var str = backend.qmlGetDeviceID()
                                if(str !== "*")
                                {
                                    netFileDialog.doOneTimeSearch(str)
                                }
                            }
                        }
                    }
					Button
					{
						id:bt_OpenFile
						anchors.top: parent.top
						width: 88
						height: 25
                        text: "打开文件"
						font.pixelSize: 14
						font.family: "Arial"

						// 设置按钮文本
						contentItem: Text
						{
							text: bt_OpenFile.text
							font: bt_OpenFile.font
							color: "black"
							horizontalAlignment: Text.AlignHCenter
							verticalAlignment: Text.AlignVCenter
							elide: Text.ElideRight
						}
						onClicked:
						{
							fileDialog.open()
						}
					}
					Button
					{
						id:bt_Update
						anchors.top: parent.top
						width: 88
						height: 25
						text:"更新"
						font.pixelSize: 14
						font.family: "Arial"
						contentItem: Text
						{
							text: bt_Update.text
							font: bt_Update.font
							color: "black"
							horizontalAlignment: Text.AlignHCenter
							verticalAlignment: Text.AlignVCenter
							elide: Text.ElideRight
						}
						onClicked:
						{
							notifyStartUpdate(1);
						}
					}
				}
			}

			Item
			{
				id: item_TextOut
				anchors.top: item_top.bottom
				anchors.bottom: item_Prog.top
				anchors.bottomMargin: 8
				width: 362

				Rectangle
				{
					anchors.fill: parent
					color: "white"
					border.color: "grey"

					ScrollView
					{
						id:scrollView
						anchors.fill: parent
						clip: true
						ScrollBar.vertical.wheelEnabled: true
						ScrollBar.vertical.policy: ScrollBar.AlwaysOn

						TextArea
						{
							id: outputTextArea
							height: parent.height
							width : parent.width
							padding: 2
							textFormat: Text.RichText   //文本显示的格式为富文本
							font.family: "Arial"
							font.pointSize: 16
							color: "blue"
							mouseSelectionMode: TextEdit.SelectCharacters
							readOnly: true
							wrapMode:TextEdit.WrapAnywhere  //决定如何拆行 不考虑单词边界
							text:""
							selectByMouse:true
						}
					}
				}
			}



			//进度条
			Item
			{
				id:item_Prog
				anchors.bottom: item_bottom.top
				anchors.bottomMargin: 13
				width: 362
				height: 25

				ProgressBar
				{


					id:update_Prog
					value: 0
					anchors.fill: parent


					background: Rectangle
					{
						//背景项
						implicitWidth: update_Prog.width
						implicitHeight: update_Prog.height
						border.color:"grey"
						border.width:1
						color: Qt.rgba(240/255 , 240/255 , 240/255 , 1.0)
						radius: 3 //圆滑度
					}

					contentItem: Item
					{   //内容项
						Rectangle
						{
							width: update_Prog.visualPosition * update_Prog.width
							height: update_Prog.height
							radius: 2
							color: "green"
						}
					}
				}
			}

			//下面三个按键
			Item
			{
				id:item_bottom
				anchors.bottom: parent.bottom
				height: 40
				Row
				{
					anchors.topMargin: 23
					spacing: 47
					ComboBox
					{
						id: comb_Language
						anchors.top: parent.top
						width: 91
						height: 25



						currentIndex: 0
						font.pixelSize: 14
						font.family: "Arial"

						model: ListModel
						{
							id: diskModel
						}

						onCurrentIndexChanged:
						{
							backend.qml_LangageSelectChange(currentIndex);
						}

						delegate: ItemDelegate
						{
							width:comb_Language.width
							contentItem: Text
							{
								text:modelData
								color:"black"
								font.pixelSize:14
								font.family: "Arial"
								elide:Text.ElideRight
								verticalAlignment:Text.AlignVCenter
							}

							highlighted: comb_Language.highlightedIndex === index
						}

						indicator: Canvas
						{
							   id: canvas
							   x: comb_Language.width - width - comb_Language.rightPadding
							   y: comb_Language.topPadding + (comb_Language.availableHeight - height) / 2
							   width: 12
							   height: 8
							   contextType: "2d"

							   Connections {
								   target: comb_Language
								   function onPressedChanged() { canvas.requestPaint(); }
							   }

							   onPaint: {
								   context.reset();
								   context.moveTo(0, 0);
								   context.lineTo(width, 0);
								   context.lineTo(width / 2, height);
								   context.closePath();
								   context.fillStyle = comb_Language.pressed ? "#505050" : "#000000";
								   context.fill();
							   }
						   }

					}
					Button
					{
						id:bt_help
						anchors.top: parent.top
						width: 88
						height: 25
						text: "帮助"
						font.pixelSize: 14
						font.family: "Arial"
						contentItem: Text
						{
							text: bt_help.text
							font: bt_help.font
							color: "black"
							horizontalAlignment: Text.AlignHCenter
							verticalAlignment: Text.AlignVCenter
							elide: Text.ElideRight
						}
						onClicked:
						{
							Qt.openUrlExternally("https://" + help_website);
						}
					}
					Button
					{
						id:bt_exit
						anchors.top: parent.top
						width: 88
						height: 25
						text: "退出"
						font.pixelSize: 14
						font.family: "Arial"
						contentItem: Text
						{
							text: bt_exit.text
							font: bt_exit.font
							color: "black"
							horizontalAlignment: Text.AlignHCenter
							verticalAlignment: Text.AlignVCenter
							elide: Text.ElideRight
						}
						onClicked:
						{
							backend.windowsClose();
							Qt.quit()
                            // console.log("窗口关闭！！！")
						}
					}
				}
			}
		}

	}

	//打开路径对话框
	FileDialog
	{
		id:fileDialog
		file: StandardPaths.standardLocations(StandardPaths.DesktopLocation)[0]
		//文件名过滤器
		nameFilters: ["Firmware Files (*.bin)"]

		onAccepted:
		{
			backend.openFirwareFile(fileDialog.file);
		}
	}


    Qml_DeviceSelect
    {
        id:netFileDialog

        function openProgress()
        {
            //设置居中
            gidDownloadProgress.x = netFileDialog.x + (netFileDialog.width - gidDownloadProgress.width) / 2
            gidDownloadProgress.y = netFileDialog.y + (netFileDialog.height - gidDownloadProgress.height) / 2

            gidDownloadProgress.progressValue = 0;
            //打开
            gidDownloadProgress.visible = true;
        }
    }


    Qml_DownLoadProgress
    {
        id:gidDownloadProgress
    }

	Connections
	{
		target: backend

		//显示输出信息
		function onSigOutputMsg(Clolr , outputMsg)
		{
			if(Clolr === "red")
			{
				outputTextArea.append("<span style='color: red; font-family: Arial; font-size: 18px;'>" + outputMsg + "</span>")
			}
			else if(Clolr === "black")
			{
				outputTextArea.append("<span style='color: black; font-family: Arial; font-size: 18px;'>" + outputMsg + "</span>")
			}
			//移动到最后一行
			scrollView.ScrollBar.vertical.position = 1.0 - scrollView.ScrollBar.vertical.size

			if(outputTextArea.lineCount > 1000)
			{
				outputTextArea.text = ""
			}
		}

		// USB连接状态更改
		function onSigUsbConnectStatus(Status)
		{
			if(Status === 0)
			{
				img_ConnectStatus.source = usbIconOff
			}
			else
			{
				img_ConnectStatus.source = usbIconOn
			}
		}

		// 进度条的更改
		function onSigProgressValue(value)
		{
			update_Prog.value = value / 100.0;
		}

		//更新语言
        function onSiglangageChange(strTitle , strOpenfile , strUpdate , strHelp , strExit , strNetFile)
		{
			rootWindow.title = strTitle
			bt_OpenFile.text = strOpenfile
			bt_Update.text = strUpdate
			bt_help.text = strHelp
			bt_exit.text = strExit
            bt_netFile.text = strNetFile

		}

		//更新语言选择框
		function onSigInitLangageComboBox(list , len , currentSelect)
		{
			diskModel.clear();

			for(var i = 0 ; i<len ; i++)
			{
				diskModel.append({text: list[i]})
			}
			comb_Language.currentIndex = currentSelect;
		}

		//更新帮助按键的网址
		function onSigSetHelpBtnMeg(website)
		{
			help_website = website;
		}

        function onSigProgressCtrl(percent , isClose)
        {
            if(isClose === true)
            {
                gidDownloadProgress.progressValue = percent;

                if(isdownJsonFile)
                {
                    isdownJsonFile = false
                }
                else
                {
                    delay(function()
                    {
                       gidDownloadProgress.visible = false
                        netFileDialog.visible = false
                    }, 300);
                }
            }
            else
            {
                gidDownloadProgress.progressValue = percent;

            }
        }
	}

	function outputMsg(Clolr , outputMsg)
	{
		if(Clolr === "red")
		{
			outputTextArea.append("<span style='color: red; font-family: Arial; font-size: 18px;'>" + outputMsg + "</span>")
		}
		else if(Clolr === "black")
		{
			outputTextArea.append("<span style='color: black; font-family: Arial; font-size: 18px;'>" + outputMsg + "</span>")
		}
		//移动到最后一行
		scrollView.ScrollBar.vertical.position = 1.0 - scrollView.ScrollBar.vertical.size

        // console.log(outputTextArea.lineCount)
		if(outputTextArea.lineCount > 1000)
		{
			outputTextArea.text = ""
		}
	}









}
