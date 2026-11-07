// ModalWindow.qml
import QtQuick 2.15
import QtQuick.Controls 1.5
import QtQuick.Controls 2.15
import TreeViewModel 1.0

ApplicationWindow
{
    id: root

	modality: Qt.ApplicationModal
    width: 500
    height: 300
    minimumWidth: 500
    minimumHeight: 320
    // maximumWidth: 500
    // maximumHeight: 320
    visible: false // 默认不显示弹窗
    title: "选择固件文件"

    // property int fountSize1: 16
    // property int fountSize2: 14

    // property string detailMsgUrl: "http://www.baidu.com/"
    property string detailMsgUrl: ""

    property int selectFileNumber: -1



    property alias searchInputText: gidTextInputSearch.text






    function treeViewExpandAll()
    {
        //*****************************************
        //展开全部

        var cnt0 = myModel.rowCount()
        var idx0 =  myModel.index(0,0);
        //第一层
        for(var i0 = 0; i0 < cnt0; i0++)
        {
            var idx1 = myModel.index(i0,0);
            var cnt1 = myModel.rowCount(idx1)
            gidTreeView.expand(idx1)

            //第二层
            for(var i1 = 0; i1 < cnt1; i1++)
            {
                var idx2 = myModel.index(i1 , 0 , idx1);
                var cnt2 = myModel.rowCount(idx2)
                gidTreeView.expand(idx2)
                //第三层
                for(var i2 = 0; i2 < cnt2; i2++)
                {
                    var idx3 = myModel.index(i2 , 0 , idx2);
                    var cnt3 = myModel.rowCount(idx3)
                    gidTreeView.expand(idx3)
                }
            }
        }
    }




    // 弹窗背景
    Rectangle
    {
        id:gidBackGroundRec
        anchors.fill: parent
        color: Qt.rgba(240/255 , 240/255 , 240/255 , 1.0)

        Text {
            x:20
            y:10
            id: gidTextCurSelectDevice

            font.pixelSize: 14
            // font.family: "Microsoft Yahei"
            text: qsTr("选中的固件：")
        }

        Row
        {
            id:gidRowSearch
            x:20
            y:24 + 10
            spacing: 8
            Text
            {
                height: 20
                id: gidTextSearch
                text: qsTr("查找:")
                font.pixelSize: 14
                // font.family: "Microsoft Yahei"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Rectangle
            {
                anchors.verticalCenter: parent.verticalCenter
                width: 150
                height: 20
                border.color: "grey"
                TextInput
                {
                    id:gidTextInputSearch
                    anchors.fill: parent
                    anchors.margins: 2
                    clip: true
                    font.pixelSize: 16
                    selectByMouse: true

                    onEditingFinished:
                    {
                        focus = false
                    }

                    onAccepted:
                    {
                        backend.qml_SearchButtonClicked(gidTextInputSearch.text);
                        treeViewExpandAll()
                    }
                }
            }

            //查找按键
            Image
            {
                anchors.verticalCenter: parent.verticalCenter
                height: 20
                width: 20
                id: buttonImage
                source: "qrc:/img/img_search.png"

                MouseArea
                {
                   id: mouseArea
                   anchors.fill: parent
                   hoverEnabled:true

                   onClicked:
                   {
                        //过虑
                        backend.qml_SearchButtonClicked(gidTextInputSearch.text);

                       treeViewExpandAll()


                   }
                   onReleased:
                   {
                       buttonImage.source = "qrc:/img/img_search.png";  // 恢复为正常状态
                   }
                   onPressed:
                   {
                       buttonImage.source = "qrc:/img/img_search_down.png";  // 按下时更换图像
                   }
                   onHoveredChanged:
                   {
                       if (mouseArea.containsMouse)
                       {
                           buttonImage.source = "qrc:/img/img_search_over.png";  // 悬停时改变图像
                       }
                       else
                       {
                           buttonImage.source = "qrc:/img/img_search.png";  // 恢复正常图像
                       }
                   }
                }
            }
        }



        Rectangle
        {
            id:gidRecTreeList
            x:20
            anchors.top: gidRowSearch.bottom
            anchors.topMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            width: (parent.width/2 - 20 - 10)

            border.width: 1
            border.color: "grey"
            color: "white"

            TreeView
            {
                id:gidTreeView
                anchors.fill: parent
                model: myModel
                alternatingRowColors : true     //交替显示行颜色。奇偶行颜色不一样。
                backgroundVisible: false        //是否显示背景
                headerVisible: false            //是否显示表头
                verticalScrollBarPolicy: ScrollBar.AlwaysOn
                horizontalScrollBarPolicy: ScrollBar.AlwaysOn
                TableViewColumn
                {
                    title: "Name"
                    role: "name"
                    width: 600
                }

                //选择改籴
                onCurrentIndexChanged:
                {
                    var index = currentIndex
                    var str1 = myModel.data(index, TreeViewModel.NAME)
                    var parent1 = myModel.parent(index);
                    var str2 = myModel.data(parent1, TreeViewModel.NAME)
                    var parent2 = myModel.parent(parent1);
                    var str3 = myModel.data(parent2, TreeViewModel.NAME)
                    var parent3 = myModel.parent(parent2);
                    var str4 = myModel.data(parent3, TreeViewModel.NAME)

                    var NumberStr = myModel.data(index, TreeViewModel.STATE)


                    var isDevice = true;
                    if(NumberStr === "" || NumberStr === undefined)
                    {
                        isDevice = false;
                        selectFileNumber = -1;
                        gidBtnOpenFireware.enabled = false;
                        gidBtnDetailChangeList.enabled = false;
                    }
                    else
                    {
                        selectFileNumber = Number(NumberStr);
                        gidBtnOpenFireware.enabled = true;
                        gidBtnDetailChangeList.enabled = true;
                    }
                    backend.qml_TreeViewSelectDevice(isDevice , Number(NumberStr))
                }

                onDoubleClicked:
                {
                    var NumberStr = myModel.data(index, TreeViewModel.STATE)

                    if(NumberStr === "")
                    {
                        if(isExpanded(index))
                        {
                            collapse(index)
                        }
                        else
                        {
                            expand(index)
                        }
                    }
                }
            }
        }

        Rectangle
        {
            id:gidRecDetailMsg
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.top: gidRowSearch.bottom
            anchors.topMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 90
            width: (parent.width/2 - 20 - 10)
            border.width: 1
            border.color: "grey"
            // color: "transparent"
            color: "white"
            clip: true

            ScrollView
            {
                anchors.fill: parent
                ScrollBar.vertical.policy: ScrollBar.AlwaysOn

                Column
                {
                    enabled: false
                    spacing: 5
                    Text
                    {
                        id: gidTextMsg_Description
                        x:10
                        text: qsTr("固件说明：")
                        font.pixelSize: 14
                        font.family: "Microsoft Yahei"
                        color: Qt.rgba(51/255,51/255,51/255,1)

                    }

                    Text
                    {
                        id: gidTextMsg_DescriptionChild
                        x:10
                        width: gidRecDetailMsg.width - 20
                        text: qsTr("版本号V1.2.6，发布时间：2024/10/23")
                        font.pixelSize: 12
                        font.family: "Microsoft Yahei"
                        wrapMode: Text.WordWrap //换行
                        color: Qt.rgba(102/255,102/255,102/255,1)
                    }

                    Text
                    {
                        id: gidTextMsg_Remark
                        x:10
                        text: qsTr("注意事项：")
                        font.pixelSize: 14
                        font.family: "Microsoft Yahei"
                        color: Qt.rgba(51/255,51/255,51/255,1)
                    }
                    Text
                    {
                        id: gidTextMsg_RemarkChild
                        x:10
                        width: gidRecDetailMsg.width - 20
                        text: qsTr("X3N 无线发射器, 适用于尼康相机")
                        font.pixelSize: 12
                        font.family: "Microsoft Yahei"
                        wrapMode: Text.WordWrap //换行
                        color: Qt.rgba(102/255,102/255,102/255,1)
                    }

                    Text
                    {
                        id: gidTextMsg_upeateContent
                        x:10
                        text: qsTr("更新内容：")
                        font.pixelSize: 14
                        font.family: "Microsoft Yahei"
                        color: Qt.rgba(51/255,51/255,51/255,1)
                    }

                    Text
                    {
                        id: gidTextMsg_upeateContentchild
                        x:10
                        width: gidRecDetailMsg.width - 20
                        text: qsTr("1、修复在单组设置界面，调入用户预设值后导致的显示错乱、不可切换组别的问题。")
                        font.pixelSize: 12
                        font.family: "Microsoft Yahei"
                        wrapMode: Text.WordWrap //换行
                        color: Qt.rgba(102/255,102/255,102/255,1)
                    }



                }
            }



        }

        Button
        {
            id:gidBtnDetailChangeList
            anchors.right: gidRecDetailMsg.right
            anchors.top: gidRecDetailMsg.bottom
            anchors.topMargin: 10
            width:100
            height: 30
            text: "详细修改历史"
            onClicked: Qt.openUrlExternally(detailMsgUrl);

        }

        Row
        {
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.horizontalCenter: gidRecDetailMsg.horizontalCenter
            spacing: 20
            Button
            {
                id:gidBtnOpenFireware
                width:60
                height: 30
                text: "打开"
                enabled: false
                onClicked:
                {
                    if(!(selectFileNumber === -1))
                    {

                        openProgress()
                        backend.qml_OpenNetFile(selectFileNumber)
                    }
                    // root.close();

                    // root.visible = false
                }

            }

            Button
            {
                id:gidBtnCancel
                width:60
                height: 30
                text: "取消"
                onClicked:  root.close()
            }
        }
    }

    Connections
    {
        target: backend

        function onSigSetCurSelectDevice(msg)
        {
            gidTextCurSelectDevice.text = msg
        }

        function onSigSetDetailMsg(description , descChild , updateContent , updateContentChild , Remark , RemarkChild , msgUrl)
        {
            gidTextMsg_Description.text = description;
            gidTextMsg_DescriptionChild.text = descChild;
            gidTextMsg_upeateContent.text = updateContent;
            gidTextMsg_upeateContentchild.text = updateContentChild;
            gidTextMsg_Remark.text = Remark;
            gidTextMsg_RemarkChild.text = RemarkChild;

            detailMsgUrl = msgUrl
        }

        //更新语言
        function onSigNetFileDialogLanguageChange(OpenStr, CancelStr, SelectedFirmwareStr, FirmwareDescStr, UpdatesStr, RemarkStr , SearchStr , UpdateDetails)
        {
            gidBtnOpenFireware.text = OpenStr
            gidBtnCancel.text = CancelStr;
            gidTextCurSelectDevice.text = SelectedFirmwareStr
            gidTextMsg_Description.text = FirmwareDescStr
            gidTextMsg_upeateContent.text = UpdatesStr
            gidTextMsg_Remark.text = RemarkStr
            gidTextSearch.text = SearchStr
            gidBtnDetailChangeList.text = UpdateDetails
        }

        function onSigNetFileDialogWindowsTitle(title)
        {
            root.title = title;
        }

    }

    function doOneTimeSearch(str)
    {
        gidTextInputSearch.text = str;
        //过虑
        backend.qml_SearchButtonClicked(gidTextInputSearch.text);
        treeViewExpandAll()
    }
}
