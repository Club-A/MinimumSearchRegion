
var changeTabContext=0;

function changeTab2(i){
    console.log("%s-右边UI tab切换","changeTab")
    if(changeTabContext===i) return;
    //var innerContent = '';
    document.getElementsByClassName('r-tab')[changeTabContext].className = 'r-tab';
    document.getElementsByClassName('r-tab')[i].className = 'r-tab selected';
    changeTabContext =i;
    switch(i){
        case 0:
            document.getElementById('tab-content1').style.display='none';
            document.getElementById('tab-content0').style.display='block';
            break;
        case 1:
            document.getElementById('tab-content1').style.display='block';
            document.getElementById('tab-content0').style.display='none';
            break;
    }
    // document.getElementById('tab-content').innerHTML = innerContent;
}
