# include <Siv3D.hpp> // Siv3D v0.6.15


// 敵の位置をランダムに作成する関数
Vec2 GenerateEnemy()
{
	// x: 50～750, y: -20 の2次元座標.
	return RandomVec2({ 50, 750 }, -20);
}

void Main()
{
	Scene::SetBackground(ColorF{ 0.1, 0.2, 0.7 });

	//const Font font{ FontMethod::MSDF, 48 };

	// 自機テクスチャ // 🧘🤖⚜
	const Texture playerTexture{ U"🧘"_emoji };

	// 敵テクスチャ
	const Texture enemyTexture{ U"👾"_emoji };

	// 自機（初期位置）.
	Vec2 playerPos{ 400, 500 };

	// 自機弾（位置）.
	Array<Vec2> playerBullets;

	// 敵（位置）.
	Array<Vec2> enemies = { GenerateEnemy() };

	// 自機のスピード.
	constexpr double PlayerSpeed = 550.0;

	// 自機ショットのスピード
	constexpr double PlayerBulletSpeed = 500.0;

	// 自機ショットのクールタイム（秒）
	constexpr double PlayerShotCoolTime = 0.1;
	// 自機ショットのクールタイムタイマー（秒）
	double playerShotTimer = 0.0;

	while (System::Update())
	{
		// 1フレームの経過時間.
		const double deltaTime = Scene::DeltaTime();

		////////////////////////
		// 毎フレームの処理.
		////////////////////////
		
		// 自機弾のタイマー処理.
		playerShotTimer = Min((playerShotTimer + deltaTime), PlayerShotCoolTime);
		// playerShotTimer += deltaTime;
		// if( playerShotTimer > PlayerShotCoolTime ){
		//   playerShotTimer = PlayerShotCoolTime;
		// }

		// 自機の移動ベクトル.
		// 入力方向→正規化→スピード係数をかけて調整→シフト入力で減速する仕組み.
		const Vec2 move = Vec2{
			(KeyRight.pressed() - KeyLeft.pressed()),
			(KeyDown.pressed() - KeyUp.pressed())
		}.setLength(deltaTime * PlayerSpeed * (KeyShift.pressed() ? 0.5 : 1.0));

		// 自機の移動.
		playerPos.moveBy(move).clamp(Scene::Rect());

		// 自機弾の発射
		if (PlayerShotCoolTime <= playerShotTimer)
		{
			playerShotTimer -= PlayerShotCoolTime;
			playerBullets << playerPos.movedBy(0, -50);
		}

		// 自機弾の発射.
		if (PlayerShotCoolTime <= playerShotTimer)
		{	// 発射タイミング
			playerShotTimer -= PlayerShotCoolTime;		// 次回発射までの時間計算.
			playerBullets << playerPos.movedBy(0, -50);	// 配列に追加.
		}
		// 自機弾を移動.
		for (auto& playerBullet : playerBullets)
		{
			playerBullet.y += (deltaTime * -PlayerBulletSpeed);
		}
		// 画面外に出た自機弾を削除.
		playerBullets.remove_if([](const Vec2& b) { return (b.y < -40); });

		////////////////////////
		// 描画.
		////////////////////////

		// 自機を描画.
		//playerTexture.resized(80).flipped().drawAt(playerPos);
		playerTexture.resized(80).drawAt(playerPos);

		// 自機弾を描画.
		for (const auto& playerBullet : playerBullets)
		{
			Circle{ playerBullet, 8 }.draw(Palette::Orange);
		}

	}
}

//
// - Debug ビルド: プログラムの最適化を減らす代わりに、エラーやクラッシュ時に詳細な情報を得られます。
//
// - Release ビルド: 最大限の最適化でビルドします。
//
// - [デバッグ] メニュー → [デバッグの開始] でプログラムを実行すると、[出力] ウィンドウに詳細なログが表示され、エラーの原因を探せます。
//
// - Visual Studio を更新した直後は、プログラムのリビルド（[ビルド]メニュー → [ソリューションのリビルド]）が必要な場合があります。
//
