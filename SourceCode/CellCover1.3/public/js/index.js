function modelTost() {
    let tem = "";
    let imgArr = [];
    let tems = '<div class="modelTost">' +
        '<div class="modelTostChile">' +
        '<p>层次关系展示 <span class="modelClose">×</span></p>' +
        '<div class="swiper-container">' +
        '<div class="swiper-wrapper"></div>' +
        '<div class="swiper-button-next"></div>' +
        '<div class="swiper-button-prev"></div>' +
        '</div>' +
        '</div>' +
        '</div>'

    function imgClick() {
        $('img').click(function () {
            tem = ''
            imgArr = [];
            let slideIndex = 0;
            let attrs = $(this).attr('src');
            $('body').append(tems);
            let imgArrList = $(this).parents('.modelTostParents').find('img');
            for (var i = 0; i < imgArrList.length; i++) {
                imgArr.push(imgArrList.eq(i).attr('src'))
            }
            imgArr.forEach((item, index) => {
                if (attrs === item) {
                    slideIndex = index
                }
            })
            for (var i = 0; i < imgArrList.length; i++) {
                tem += "<div class='swiper-slide'><img src='" + imgArr[i] + "'></div>"
            }
            $(".swiper-wrapper").append(tem);
            $('.modelTost').fadeIn(500);

        })
    }

    $('body').on('click', '.modelClose', function () {
        $('.modelTost').remove();
    })
    imgClick()
}