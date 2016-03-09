<?php

/**
 * Popup’s callback function for form.
 */
function my_popup_callback ($js = NULL) {
    if (!$js) {
        // Якщо JavaScript вимкнено — то виводимо форму без попапа.
        return drupal_get_form('id_example_form');
    }

    ctools_include('ajax');
    ctools_include('modal');

    // Задаємо початкові налаштування форми.
    $form_state = array(
        'ajax' => TRUE,
        'title' => t('Form title’'),
    );

    $output = ctools_modal_form_wrapper('id_example_form', $form_state);

    if (!empty($form_state['executed'])) {

        $number = $form_state['values']['number'];
        $teaser = $form_state['values']['teaser'];
        $text = $form_state['values']['text'];


        if (!is_numeric($number) || $number > 2147483647 || strlen($number) == 0) {
            form_set_error('number', t('Value in field "Number" is wrong.'));
            $output = array();
            $output[] = ctools_modal_command_display(t("Sending form"), "Error...");
            $output[] = ctools_ajax_command_reload();
        }

        elseif (strlen($teaser) > 150  || strlen($teaser) == 0) {
            form_set_error('number2', t('Value in field "Teaser" is wrong.'));
            $output = array();
            $output[] = ctools_modal_command_display(t("Sending form"), "Error...");
            $output[] = ctools_ajax_command_reload();
        }

        elseif (strlen($text) == 0) {
            form_set_error('number', t('Value in field "Text" is wrong.'));
            $output = array();
            $output[] = ctools_modal_command_display(t("Sending form"), "Error...");
            $output[] = ctools_ajax_command_reload();
        }else {


            db_insert('custom_table')
                ->fields(array(
                    'number' => $number,
                    'teaser' => $teaser,
                    'text' => $text
                ))
                ->execute();

            $output = array();
            $output[] = ctools_modal_command_display(t("Sending form"), "Save...");
            $output[] = ctools_ajax_command_reload();
            drupal_set_message(t('DATA INSERT'));
        }

    }

    print ajax_render($output);

    drupal_exit();
}


/**
 * Popup’s example form.
 */
function id_example_form($form, $form_state) {
    $form = array();

    $form['number'] = array(
        '#type' => 'textfield',
        '#title' => t('Number'),
        '#default_value' => '',
        '#size' => 40,
    );

    $form['teaser'] = array(
        '#type' => 'textfield',
        '#title' => t('Teaser'),
        '#size' => 40,
    );

    $form['text'] = array(
        '#title'=> t('Text'),
        '#type' => 'textarea',
        '#attributes' => array('class' => array('my_database_textarea')),
        '#description' => t ('Default bottoom text'),
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('OK'),
    );

    //$form['#action'] = url("my_database/testtest");

    return $form;

}


/**
 *Popup’s setings function.
 */
function my_ctools_popup_style() {
    static $added = FALSE;
    if ($added == FALSE) {
        $added = TRUE;
        // Підключаємо необхідні бібліотеки
        ctools_include('modal');
        ctools_include('ajax');
        ctools_modal_add_js();
        // Задаємо налаштування для попапа
        $popup_style = array(
            'first-popup-style' => array(
                'modalSize' => array(
                    'type' => 'fixed', // Тип попапа.
                    'width' => 475, // Ширина
                    'height' => 'auto', // Висота
                    'addHeight' => 700, // Максимальна висота
                ),
                'modalOptions' => array(
                    'opacity' => (float) 0.8, // Прозорість заднього фону
                    'background-color' => '#084b57', // Колір заднього фону
                ),
                'closeText' => 'xzz', // Текст для кнопки «close»
                'loadingText' => 'Loading', // Текст при завантаженні попапа
                'animation' => 'fadeIn', // Тип анімації
                'modalTheme' => 'my_custom_theme', // Назва теми, яку слід підключити
                'animationSpeed' => 'fast', // Швидкість анімації попапа
            ),
        );
        drupal_add_js($popup_style, 'setting'); // Підключаємо налаштування
        ctools_add_js('my_popup_style', 'my_database'); // Підключаємо тему (перший параметр – це назва файла теми, другий - назва модуля, в якому цей файл знаходиться)
    }
}


/**
 * function for return link
 */
function my_popup_page() {
    my_ctools_popup_style();
    return ctools_modal_text_button(t('Add NEW'), 'first-popup/nojs', t('test popup'), 'ctools-modal-first-popup-style');
}

/**
 * Submit function for id_example_form.
 */
function id_example_form_submit($form, $form_state) {

}





