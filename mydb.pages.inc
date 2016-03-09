<?php

function my_database_table_data_page() {
    $header = array(t('ID'), t('Number'), t('Teaser'), t('Text'), t('Actions'));
    $rows = array();
    $query = db_select('custom_table', 't')
        ->fields('t', array('id', 'number', 'teaser', 'text'))
        ->orderBy('t.id', 'DESC')
        ->execute();


    while ($value = $query->fetchAssoc()) {
        $rows[] = array(
            $value['id'],
            $value['number'],
            $value['teaser'],
            $value['text'],
            '<a href="/my_database/?id='.$value['id'].'">Edit</a>'.' / '.l(t('Delete'), "my_database/{$value['id']}/delete"),
        );
    }
    return theme('table', array('header' => $header, 'rows' => $rows));
}


function my_database_form($form, $form_state) {


    $number = '';
    $teaser = '';
    $text = '';
    $button = t('Record');

    if(isset($_GET['id'])){

        $idd = $_GET['id'];

        $query = db_select('custom_table', 't')
            ->fields('t', array('id', 'number', 'teaser', 'text'))
            ->condition('t.id', $idd)
            ->execute()
            ->fetchAssoc();

        $number = t($query['number']);
        $teaser = t($query['teaser']);
        $text = t($query['text']);
        $button = t('Update');
    }


    $form = array();


    $form['number'] = array(
        '#type' => 'textfield',
        '#title' => t('Number'),
        '#default_value' => '',
        '#size' => 40,
        '#default_value' => $number,
    );

    $form['teaser'] = array(
        '#type' => 'textfield',
        '#title' => t('Teaser'),
        '#size' => 40,
        '#default_value' => $teaser,
    );

    $form['text'] = array(
        '#title'=> t('Text'),
        '#type' => 'textarea',
        '#description' => t ('Default bottoom text'),
        '#default_value' => $text,
    );


    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => $button,
    );

    return $form;
}


/**
 * Validate function for my_database_form.
 */
function my_database_form_validate($form, &$form_state) {

    $number = $form_state['values']['number'];
    $teaser = $form_state['values']['teaser'];
    $text = $form_state['values']['text'];

    if (!is_numeric($number) || $number > 2147483647 || strlen($number) == 0) {
        form_set_error('number', t('Value in field "Number" is wrong.'));
    }

    if (strlen($teaser) > 150  || strlen($teaser) == 0) {
        form_set_error('number2', t('Value in field "Teaser" is wrong.'));
    }

    if (strlen($text) == 0) {
        form_set_error('number', t('Value in field "Text" is wrong.'));
    }

}

/**
 * Submit function for my_database_form.
 */
function my_database_form_submit($form, &$form_state) {
    $number = $form_state['values']['number'];
    $teaser = $form_state['values']['teaser'];
    $text = $form_state['values']['text'];


    /**
     * UPDATE if GET['id']
     */
    if(!isset($_GET['id'])) {
        db_insert('custom_table')
            ->fields(array(
                'number' => $number,
                'teaser' => $teaser,
                'text' => $text
            ))
            ->execute();

        drupal_set_message(t('DATA INSERT'));
    }else{

        db_update('custom_table')
            ->fields(array('number' => $number,'teaser'=>$teaser,'text'=>$text))
            ->condition('id', $_GET['id'])
            ->execute();

        drupal_set_message(t('DATA UPDATE'));

        drupal_goto('/my_database/list');

    }
}

/**
 * Submit form text.
 */
function my_database_delete_page_title($id) {
    $placeholders = array(':id' => $id);
    $number = db_query("SELECT `number` FROM {custom_table} WHERE `id` = :id", $placeholders)->fetchField();
    return t('Delete item with number "@num"?', array('@num' => $number));
}

/**
 * Submit form
 */
function my_database_delete_form($form, $form_state) {
    $form = array();
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Yes'),
    );
    $form['cancel'] = array(
        '#markup' => l(t('Cancel'), 'my_database/list'),
    );
    return $form;
}

/**
 * Submit
 */
function my_database_delete_form_submit($form, &$form_state) {
    $id = $form_state['build_info']['args'][0];
    $result = db_delete('custom_table')
        ->condition('id', $id)
        ->execute();
    $placeholders = array('@id' => $id);
    if ($result) {
        drupal_set_message(t('Record @id successfully removed.', $placeholders));
    }
    else {
        drupal_set_message(t('Record @id is undefined.', $placeholders), 'warning');
    }
    $form_state['redirect'] = 'my_database/list';
}



